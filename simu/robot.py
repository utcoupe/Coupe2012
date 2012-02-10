# -*- coding: utf-8 -*-

import random
import math

import pymunk

from define import *

import collections

SEP = '.'

class Robot(EngineObject):
	def __init__(self, mass, position, color, team, poly_points, *custom_objects):
		EngineObject.__init__(self, *custom_objects)
		self.t = POLY
		self.collision_type = COLLTYPE_ROBOT
		
		self.mass = mass
		self.posinit = position
		self.color = color
		self.poly_points = list(poly_points)
		self.team = team

		self.goals = []
		self.to_send = collections.deque()
		self.shift_on = False
		self.canal_asserv = CANAL_ASSERV

		self.stop = False

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
				self.send(self.canal_asserv,"reponse","goto",x,y,v)
			else:
				a = math.atan2(dy,dx)
				vx = dx * v / d
				vy = dy * v / d
				self.body._set_velocity((vx,vy))
				self.body._set_angle(a)

	def send(self, canal, *msg):
		self.to_send.append((self.canal_asserv,SEP.join(map(str, msg))))
	
	def cmd_goto(self, id_msg, x, y, v):
		"""
		@param x mm
		@param y mmm
		@param v ~~
		"""
		self.send_canal_asserv(id_msg,"recu")
		self.goals.append(mm_to_px(x,y,v))
	
	def cmd_gotor(self, id_msg, x, y, v): pass

	def cmd_turn(self, id_msg, a, v): pass

	def cmd_turnr(self, id_msg, a, v): pass

	def cmd_acalib(self, id_msg, c): pass

	def cmd_cancel(self, id_msg):
		self.goals = []
		self.send_canal_asserv(id_msg,"recu")

	def cmd_stop(self, id_msg):
		self.stop = True
		self.send_canal_asserv(id_msg,"recu")

	def cmd_resume(self, id_msg):
		self.stop = False
		self.send_canal_asserv(id_msg,"recu")
	
	def cmd_pos(self, id_msg):
		self.send_canal_asserv(id_msg, self.x(), self.y(), self.a())

	def compute_msg(self, *args):
		return SEP.join(map(lambda x: str(x), args))

	def send_canal_asserv(self, *args):
		self.send(self.canal_asserv, self.compute_msg(*args))

	
			
