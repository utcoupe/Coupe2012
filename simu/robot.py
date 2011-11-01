# -*- coding: utf-8 -*-

import random
import math

import pymunk as pm

from define import *
from pince import *

import collections

class Robot(EngineObject):
	def __init__(self):
		self.pince = Pince()
		self.t = POLY
		self.color = "black"
		
		self.mass = 10
		self.poly_points = list(map(mm_to_px,[(100,0),(160,0),(280,90),(280,230),(160,320),(100,320),(0,290),(0,30)]))

		self.goals = []

		self.to_send = collections.deque()

	
	def get_msg(self):
		if self.to_send:
			return self.to_send.popleft()
		else:
			return (None,None)

	def onMouseEvent(self, x, y):
		"""
		@param x px
		@param y px
		"""
		self.cmd_goto(*list(map(px_to_mm,(x,y,500))))
	
	def cmd_goto(self, x, y, v):
		"""
		@param x mm
		@param y mmm
		@param v ~~
		"""
		self.to_send.append((CANAL_ASSERV,"goto ok"))
		self.goals.append(tuple(map(mm_to_px,(x,y,v))))
	
	
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
				x,y,v = self.goals.pop(0)
				self.body._set_velocity((0,0))
				self.to_send.append((CANAL_ASSERV,"goal ok : {0} {1} {2}".format(x,y,v)))
			else:
				a = math.atan2(dy,dx)
				vx = dx * v / d
				vy = dy * v / d
				self.body._set_velocity((vx,vy))
				self.body._set_angle(a)
			
			
