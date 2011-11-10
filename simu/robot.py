# -*- coding: utf-8 -*-

import random
import math


from define import *

import collections


class Robot(EngineObject):
	def __init__(self, mass, position, color, poly_points):
		self.t = POLY
		self.collision_type = COLLTYPE_ROBOT
		
		self.mass = mass
		self.posinit = position
		self.color = color
		self.poly_points = list(poly_points)

		self.goals = []

		self.to_send = collections.deque()

		self.shift_on = False

		self.canal_asserv = CANAL_ASSERV
	
	def get_msg(self):
		if self.to_send:
			return self.to_send.popleft()
		else:
			return (None,None)

	def step(self, dt):
		self.body._set_torque(0)
		self.body._set_angular_velocity(0)
		if self.goals:
			gx,gy,v = self.goals[0]
			x,y = self.body.position
			dx = gx - x
			dy = gy - y
			d = math.sqrt(dx**2+dy**2)
			if d < v * dt:
				self.body._set_position((gx,gy))
				x,y,v = list(map(px_to_mm,self.goals.pop(0)))
				self.body._set_velocity((0,0))
				self.to_send.append((self.canal_asserv,"goal ok : {0} {1} {2}".format(x,y,v)))
			else:
				a = math.atan2(dy,dx)
				vx = dx * v / d
				vy = dy * v / d
				self.body._set_velocity((vx,vy))
				self.body._set_angle(a)
	
	
	def cmd_goto(self, x, y, v):
		"""
		@param x mm
		@param y mmm
		@param v ~~
		"""
		self.to_send.append((self.canal_asserv,"goto ok"))
		self.goals.append(mm_to_px(x,y,v))
	
	def cmd_gotor(self, x, y, v): pass

	def cmd_turn(self, a, v): pass

	def cmd_turnr(self, a, v): pass

	def cmd_acalib(self, c): pass

	def cmd_stop(self): pass

	def cmd_resumt(self): pass
	
	

	
			
