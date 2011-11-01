# -*- coding: utf-8 -*-

import random
import math

import pymunk as pm

from define import *
from pince import *


class Robot(EngineObject):
	def __init__(self):
		self.pince = Pince()
		self.t = POLY
		self.color = "black"
		
		self.mass = 10
		self.poly_points = list(map(mm_to_px,[(100,0),(160,0),(280,90),(280,230),(160,320),(100,320),(0,290),(0,30)]))

		self.goals = []


	def onMouseEvent(self, x, y):
		self.goto(x,y,500)
	
	def goto(self, x, y, v):
		"""
		@param x mm
		@param y mmm
		@param v ~~
		"""
		self.goals.append((x,y,v))
	
	
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
				self.goals.pop(0)
				self.body._set_velocity((0,0))
			else:
				a = math.atan2(dy,dx)
				vx = dx * v / d
				vy = dy * v / d
				self.body._set_velocity((vx,vy))
				self.body._set_angle(a)
			
			
