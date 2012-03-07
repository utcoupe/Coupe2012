# -*- coding: utf-8 -*-


import random
import math


from py3irc.mypyirc.mypyircbot import Executer
from py3irc.mypyirc.ircutils import *
from ..define import *
from ..engine.engineobject import EngineObjectPoly





class GoalPWM:
	def __init__(self, pwm):
		self.pwm = pwm

class GoalPOS:
	def __init__(self, x, y, v):
		self.pos = (x,y)
		self.v = v


class Robot(EngineObjectPoly, Executer):
	def __init__(self, *, canal_asserv, canal_others, team, posinit, mass, poly_points, typerobot, custom_objects=[]):
		color = 'blue' if team == BLUE else 'red'
		EngineObjectPoly.__init__(self,
			colltype		= COLLTYPE_ROBOT,
			mass			= mass,
			posinit			= posinit,
			color			= color,
			poly_points		= poly_points,
			custom_objects	= custom_objects
		)
		Executer.__init__(self)

		self.typerobot = typerobot

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
		self.canal_others = canal_ircnormalize(canal_asserv)
		

		# rajouts des fonctions utilisées par le bot irc
		# _cmd_asserv_<cmd> => cmd_<canal_asserv>_<cmd>
		self.transform("asserv", canal_asserv)
		self.transform("others", canal_others)

	def init(self, engine):
		self.engine = engine

	def x(self):
		return px_to_mm(self.body.position[0])

	def y(self):
		return px_to_mm(self.body.position[1])

	def a(self):
		return int(math.degrees(self.body.angle))

	def step(self, dt):
		self.body._set_torque(0)
		self.body._set_angular_velocity(0)
		if not self.stop and self.goals:
			current_goal = self.goals[0]
			if isinstance(current_goal, GoalPOS):
				gx,gy = current_goal.pos
				v = current_goal.v
				x,y = self.body.position
				dx = gx - x
				dy = gy - y
				d = math.sqrt(dx**2+dy**2)
				if d < v * dt:
					self.body._set_position((gx,gy))
					self.goals.pop(0)
					self.body._set_velocity((0,0))
				else:
					a = math.atan2(dy,dx)
					vx = v * math.cos(a)
					vy = v * math.sin(a)
					self.body._set_velocity((vx,vy))
					self.body._set_angle(a)
			elif isinstance(current_goal, GoalPWM):
				a = self.body.angle
				v = self.max_speed * current_goal.pwm / 255
				vx = v * math.cos(a)
				vy = v * math.sin(a)
				self.body._set_velocity((vx,vy))
			else:
				raise Exception("type_goal inconnu")
		else:
			self.body._set_velocity((0,0))
				
				

	def _cmd_asserv_id(self,**options):
		self.send_canal_asserv(options['id_msg'], 'asserv')

	def _cmd_asserv_ping(self, **options):
		self.send_canal_asserv(options['id_msg'], 'pong')
	
	def _cmd_asserv_goto(self, x, y, v, **options):
		"""
		@param x mm
		@param y mm
		@param v mm/s
		"""
		self.send_canal_asserv(options['id_msg'], 1)
		self.goals.append( GoalPOS(*mm_to_px(x,y,v)) )
	
	def _cmd_asserv_gotor(self, x, y, v, **options): pass

	def _cmd_asserv_turn(self, a, v, **options): pass

	def _cmd_asserv_turnr(self, a, v, **options): pass

	def _cmd_asserv_cancel(self, **options):
		self.goals = []
		self.send_canal_asserv(options['id_msg'], 1)

	def _cmd_asserv_stop(self, **options):
		self.stop = True
		self.send_canal_asserv(options['id_msg'], 1)

	def _cmd_asserv_resume(self, **options):
		self.stop = False
		self.send_canal_asserv(options['id_msg'], 1)
	
	def _cmd_asserv_pos(self, **options):
		self.send_canal_asserv(options['id_msg'], self.x(), self.y(), self.a())

	def _cmd_asserv_pwm(self, pwm, **kwargs):
		self.goals.append(GoalPWM(pwm))
		self.send_canal_asserv(kwargs['id_msg'], 1)


	def send_canal_asserv(self, *args):
		self.send(self.canal_asserv, *args)
	
	def send_canal_others(self, *args):
		self.send(self.canal_others, *args)

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
				elif KEY_CANCEL == event.key:
					self._cmd_asserv_cancel(id_msg=42)
			# keyup
			elif KEYUP == event.type:
				if KEY_STOP_RESUME == event.key:
					self._cmd_asserv_resume(id_msg=42)
			# mouse
			elif event.type == MOUSEBUTTONDOWN:
				p = event.pos
				print(px_to_mm(p[0],p[1]))
				self._cmd_asserv_goto(*px_to_mm(p[0],p[1],mm_to_px(1000)), id_msg=42)
				return True
		return False

	def _event_concerns_me(self, event):
		return self.current_team == self.team and self.typerobot == self.current_robot
	
	def __repr__(self):
		return "Robot"
			
