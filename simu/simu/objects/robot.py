# -*- coding: utf-8 -*-


import random
import math


from py3irc.mypyirc.mypyircbot import Executer
from py3irc.mypyirc.ircutils import *
from ..define import *
from ..engine.engineobject import EngineObjectPoly
from .cd import Cd
from .lingo import Lingo


def angle_diff(a, b):
	return min(2*math.pi - (b-a), b-a)

class GoalPWM:
	def __init__(self, pwm):
		self.pwm = pwm

class GoalPOS:
	def __init__(self, id_msg, x, y, v):
		"""
		@param {int:px} x
		@param {int:px} y
		@param {float:px/s} v
		"""
		self.pos = (x,y)
		self.v = v
		self.id_msg = id_msg


class GoalANGLE:
	def __init__(self, id_msg, a, v):
		"""
		@param {float:radians} a
		@param {float:px/s} v
		"""
		self.a = a
		self.v = v
		self.id_msg = id_msg


class GoalPOSR(GoalPOS): pass
class GoalANGLER(GoalANGLE): pass


class Robot(EngineObjectPoly, Executer):
	def __init__(self, *, engine, canal_asserv, canal_others, canal_visio, canal_extras,
	team, posinit, mass, poly_points, typerobot, extension_objects=[]):
		color = 'blue' if team == BLUE else 'red'
		EngineObjectPoly.__init__(self,
			engine		 	= engine,
			colltype		= COLLTYPE_ROBOT,
			mass			= mass,
			posinit			= posinit,
			color			= color,
			poly_points		= poly_points,
			extension_objects	= extension_objects
		)
		Executer.__init__(self)

		self.typerobot = typerobot

		# quand on clique ça téléporte au lieu d'envoyer un ordre à l'asservissement
		self.mod_teleport = False

		# marche arrière ou marche avant ?
		self.mod_recul = False

		# vitesse maximale (quand pwm=255)
		self.max_speed = 1000
		
		# team
		self.team = team

		# les goals
		self.goals = []
		
		self.stop = False

		# touche shift enfoncée ?
		self.current_robot	= BIG
		self.current_team	= BLUE
		self.stop			= False

		# être sûr que le canal commance par #
		self.canal_asserv = canal_ircnormalize(canal_asserv)
		self.canal_others = canal_ircnormalize(canal_others)
		self.canal_extras = canal_ircnormalize(canal_extras)
		self.canal_visio = canal_ircnormalize(canal_visio)
		

		# rajouts des fonctions utilisées par le bot irc
		# _cmd_asserv_<cmd> => cmd_<canal_asserv>_<cmd>
		self.transform("asserv", canal_asserv)
		self.transform("others", canal_others)
		self.transform("visio", canal_visio)
		self.transform("extras", canal_extras)

		self.body._set_velocity_func(self._my_velocity_func())

	def init(self, engine):
		self.engine = engine
	
	def x(self):
		return px_to_mm(self.body.position[0])
	
	def y(self):
		return px_to_mm(self.body.position[1])
	
	def a(self):
		return int(math.degrees(self.body.angle))
		

	def _my_velocity_func(self):
		def f(body, gravity, damping, dt):
		
			self.body._set_torque(0)
			self.body._set_angular_velocity(0)
			if not self.stop and self.goals:
				current_goal = self.goals[0]
				if isinstance(current_goal, GoalPOSR):
					x,y = self.body.position
					a = self.body.angle
					cx, cy = current_goal.pos
					dx = cx * math.cos(a) - cy * math.sin(a)
					dy = cx * math.sin(a) + cy * math.cos(a)
					self.goals[0] = GoalPOS(current_goal.id_msg, x+dx, y+dy, current_goal.v)
				elif isinstance(current_goal, GoalANGLER):
					a = current_goal.a
					ca = self.body.angle
					self.goals[0] = GoalANGLE( current_goal.id_msg, ca+a, current_goal.v )
				elif isinstance(current_goal, GoalPOS):
					gx,gy = current_goal.pos
					v = current_goal.v
					x,y = self.body.position
					dx = gx - x
					dy = gy - y
					d = math.sqrt(dx**2+dy**2)
					if d < abs(v * dt):
						self.body._set_position((gx,gy))
						self.goals.pop(0)
						self.body._set_velocity((0,0))
						self.send_canal_asserv(current_goal.id_msg, 2)
					else:
						a = math.atan2(dy, dx)
						vx = abs(v) * math.cos(a)
						vy = abs(v) * math.sin(a)
						self.body._set_velocity((vx,vy))
						if v < 0:
							a += math.pi
						self.body._set_angle(a)
				elif isinstance(current_goal, GoalPWM):
					a = self.body.angle
					v = self.max_speed * current_goal.pwm / 255
					vx = v * math.cos(a)
					vy = v * math.sin(a)
					self.body._set_velocity((vx,vy))
				elif isinstance(current_goal, GoalANGLE):
					self.body._set_angle(current_goal.a)
					self.goals.pop(0)
					self.send_canal_asserv(current_goal.id_msg, 2)
				else:
					raise Exception("type_goal inconnu")
			else:
				self.body._set_velocity((0,0))
		return f
	
	
	##
	##		ASSERV
	##
				

	def _cmd_asserv_id(self,*, id_msg=42, **options):
		self.send_canal_asserv(id_msg, 'asserv')

	def _cmd_asserv_ping(self, *, id_msg=42, **options):
		self.send_canal_asserv(id_msg, 'pong')
	
	def _cmd_asserv_goto(self, x, y, v, *, id_msg=42, **options):
		"""
		Donner l'ordre d'aller à un point
		@param x mm
		@param y mm
		@param v mm/s
		"""
		self.goals = []
		self.goals.append( GoalPOS( id_msg, *mm_to_px(x,y,v) ) )
		self.send_canal_asserv(id_msg, 1)
	
	def _cmd_asserv_gotor(self, x, y, v, *, id_msg=42, **options):
		"""
		Donner l'ordre d'aller à un point, relativement à la position actuelle
		@param x mm
		@param y mm
		@param v mm/s
		"""
		self.goals = []
		self.goals.append( GoalPOSR( id_msg, *mm_to_px(x,y,v) ) )
		self.send_canal_asserv(id_msg, 1)

	def _cmd_asserv_turn(self, a, v, *, id_msg=42, **options):
		#print("HELLO", a, self.inv_a(a))
		#self.goals.append( GoalANGLE( id_msg, math.radians(self.inv_a(a)), mm_to_px(v) ) )
		self.goals.append( GoalANGLE( id_msg, math.radians(a), mm_to_px(v) ) )
		self.send_canal_asserv(id_msg, 1)

	def _cmd_asserv_turnr(self, a, v, *, id_msg=42, **options):
		#self.goals.append( GoalANGLER( id_msg, math.radians(self.inv_a(a)), mm_to_px(v) ) )
		self.goals.append( GoalANGLER( id_msg, math.radians(a), mm_to_px(v) ) )
		self.send_canal_asserv(id_msg, 1)

	def _cmd_asserv_cancel(self, *, id_msg=42, **options):
		self.goals = []
		self.send_canal_asserv(id_msg, 1)

	def _cmd_asserv_stop(self, *, id_msg=42, **options):
		self.stop = True
		self.send_canal_asserv(id_msg, 1)

	def _cmd_asserv_resume(self, *, id_msg=42, **options):
		self.stop = False
		self.send_canal_asserv(id_msg, 1)
	
	def _cmd_asserv_pos(self, *, id_msg=42, **options):
		self.send_canal_asserv(id_msg, self.x(), self.y(), self.a())

	def _cmd_asserv_pwm(self, pwm_l, pwm_r, *, id_msg=42, **kwargs):
		self.goals.append(GoalPWM(pwm_l))
		self.send_canal_asserv(id_msg, 1)
	
	##
	##		EXTRAS
	##
	
	def _cmd_extras_teleport(self, x, y, a, *, id_msg=42, **kwargs):
		"""
		Téléporte le robot
		@param {int:mm} x
		@param {int:mm} y
		@param {int:degrees} a
		"""
		self.body._set_position(mm_to_px(x,y))
		self.body._set_angle(math.radians(a))
		self.send_canal_extras(id_msg, 1)
	
	##
	##		VISIO
	##
	def _cmd_visio_ping(self, id_msg=42, **kwargs):
		self.send_canal_visio(id_msg, "pong")
		
	def _cmd_visio_get(self, id_msg=42, **kwargs):
		"""
		Récupérer ce que vois la caméra
		"""
		cds = filter(lambda o: isinstance(o, Cd), self.engine.objects)
		lingos = filter(lambda o: isinstance(o, Lingo), self.engine.objects)
		dist2 = mm_to_px(800)**2
		def condition(o):
			""" assez proche et devant nous """
			v = o.pos() - self.pos()
			return v.norm2() < dist2 and abs(angle_diff(v.angle(),self.angle())) < math.radians(45)
		cds = filter(condition, cds)
		lingos = filter(condition, lingos)
		cosa = math.cos(self.angle())
		sina = math.sin(self.angle())
		def transform(o):
			""" px -> mm     &&     absolute -> relative """
			p = o.pos() - self.pos()
			x = cosa * p.x - sina * p.y
			y = sina * p.x + cosa * p.y
			return tuple(px_to_mm(p))
		cds = map(transform, cds)
		lingos = map(transform, lingos)
		self.send_canal_visio(id_msg, str(tuple(cds)).replace(' ',''), str(tuple(lingos)).replace(' ',''))

	##
	##		SEND_CANAL_X
	##
	
	def send_canal_asserv(self, id_msg, *args):
		self.send_response(self.canal_asserv, id_msg, self.compute_msg(*args))
	
	def send_canal_others(self, id_msg, *args):
		self.send_response(self.canal_others, id_msg, self.compute_msg(*args))
		
	def send_canal_extras(self, id_msg, *args):
		self.send_response(self.canal_extras, id_msg, self.compute_msg(*args))
		
	def send_canal_visio(self, id_msg, *args):
		self.send_response(self.canal_visio, id_msg, self.compute_msg(*args))


	def onEvent(self, event):
		# selection des teams et des robots
		if KEYDOWN == event.type:
			if KEY_CHANGE_TEAM == event.key:
				self.current_team = RED
				print("équipe rouge")
				return True
			elif KEY_CHANGE_ROBOT == event.key:
				self.current_robot = MINI
				print("control du mini robot")
				return True
			elif KEY_TELEPORTATION == event.key:
				self.mod_teleport = not self.mod_teleport
				return True
			elif KEY_RECUL == event.key:
				self.mod_recul = not self.mod_recul
		elif KEYUP == event.type:
			if KEY_CHANGE_TEAM == event.key:
				print("équipe bleu")
				self.current_team = BLUE
				return True
			elif KEY_CHANGE_ROBOT == event.key:
				self.current_robot = BIG
				print("control gros robot")
				return True

		# actions
		if self._event_concerns_me(event):
			# keydown
			if KEYDOWN == event.type:
				if KEY_STOP_RESUME == event.key:
					self._cmd_asserv_stop(id_msg=42)
					return True
				elif KEY_CANCEL == event.key:
					self._cmd_asserv_cancel(id_msg=42)
					return True
			# keyup
			elif KEYUP == event.type:
				if KEY_STOP_RESUME == event.key:
					self._cmd_asserv_resume(id_msg=42)
			# mouse
			elif MOUSEBUTTONDOWN == event.type:
				p = event.pos
				p_mm = px_to_mm(p)
				print(p_mm)
				if self.mod_teleport:
					self._cmd_extras_teleport(p_mm[0], p_mm[1], 0)
				else:
					v = mm_to_px(1000) * (-1 if self.mod_recul else 1)
					self._cmd_asserv_goto(*px_to_mm(p[0],p[1],v), id_msg=42)
				return True
		return False

	def _event_concerns_me(self, event):
		return self.current_team == self.team and self.typerobot == self.current_robot
	
	def __repr__(self):
		return "Robot"

	def inv_a(self, a):
		return -a	
