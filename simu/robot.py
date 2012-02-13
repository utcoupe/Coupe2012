# -*- coding: utf-8 -*-


import sys
import os
sys.path.append(os.path.join("..","lib","py3irc"))


import random
import math
import collections

import pymunk


from mypyircbot import *


from define import *


SEP = '.'

class Robot(EngineObject):
	def __init__(self, canal_asserv, team, position, mass, color, poly_points, *custom_objects):
		EngineObject.__init__(self, *custom_objects)

		# variables requises dans un EngineObject
		self.t = POLY
		self.collision_type = COLLTYPE_ROBOT
		self.mass = mass
		self.posinit = position
		self.color = color
		self.poly_points = list(poly_points)

		# team
		self.team = team

		# les goals
		self.goals = []

		# messages à envoyer
		self.to_send = collections.deque()
		
		self.stop = False

		# touche shift enfoncée ?
		self.shift_on = False

		# canal asserv
		if canal_asserv[0] != '#': canal_asserv = '#'+canal_asserv
		self.canal_asserv = canal_asserv

		# rajouts des fonctions utilisées par le bot irc
		# cmd_asserv_<cmd> => cmd_<canal_asserv>_<cmd>
		for f_name in filter(lambda s: s.startswith('_'+channel_to_prefix_cmd("asserv")), dir(self)):
			new_f_name = replace_channel_in_f_name(f_name[1:], canal_asserv)
			setattr(self, new_f_name, getattr(self, f_name))


	def x(self):
		return px_to_mm(self.body.position[0])

	def y(self):
		return px_to_mm(self.body.position[1])

	def a(self):
		return int(math.degrees(self.body.angle))
	
	def get_msg(self):
		if self.to_send:
			return self.to_send.popleft()
		else:
			return (None,None)

	def step(self, dt):
		self.body._set_torque(0)
		self.body._set_angular_velocity(0)
		if not self.stop and self.goals:
			gx,gy,v = self.goals[0]
			x,y = self.body.position
			dx = gx - x
			dy = gy - y
			d = math.sqrt(dx**2+dy**2)
			if d < v * dt:
				self.body._set_position((gx,gy))
				x,y,v = list(map(px_to_mm,self.goals.pop(0)))
				self.body._set_velocity((0,0))
				self.send_canal_asserv("reponse","goto",x,y,v)
			else:
				a = math.atan2(dy,dx)
				vx = dx * v / d
				vy = dy * v / d
				self.body._set_velocity((vx,vy))
				self.body._set_angle(a)

	def send(self, canal, *msg):
		self.to_send.append((canal,self.compute_msg(*msg)))

	def _cmd_asserv_id(self,**options):
		self.send_canal_asserv(options['id_msg'], 'asserv')

	def _cmd_asserv_ping(self, **options):
		self.send_canal_asserv(options['id_msg'], 'pong')
	
	def _cmd_asserv_goto(self, x, y, v, **options):
		"""
		@param x mm
		@param y mmm
		@param v ~~
		"""
		self.send_canal_asserv(options['id_msg'],"recu")
		self.goals.append(mm_to_px(x,y,v))
	
	def _cmd_asserv_gotor(self, x, y, v, **options): pass

	def _cmd_asserv_turn(self, a, v, **options): pass

	def _cmd_asserv_turnr(self, a, v, **options): pass

	def _cmd_asserv_cancel(self, **options):
		self.goals = []
		self.send_canal_asserv(id_msg,"recu")

	def _cmd_asserv_stop(self, **options):
		self.stop = True
		self.send_canal_asserv(options['id_msg'],"recu")

	def _cmd_asserv_resume(self, **options):
		self.stop = False
		self.send_canal_asserv(options['id_msg'],"recu")
	
	def _cmd_asserv_pos(self, **options):
		self.send_canal_asserv(options['id_msg'], self.x(), self.y(), self.a())

	def compute_msg(self, *args):
		return SEP.join(map(lambda x: str(x), args))

	def send_canal_asserv(self, *args):
		self.send(self.canal_asserv, *args)

	
			
