# -*- coding: utf-8 -*-

from pymunk import RotaryLimitJoint, PinJoint
import pymunk
from math import *

from geometry import Vec


from ..define import *
from .robot import Robot, MINI
from ..engine.engineobject import EngineObjectPoly,EngineObjectSegment

class Palette(EngineObjectPoly):
	def __init__(self, engine, p, angle, angle_robot):
		pa = Vec(p)
		pb = pa + mm_to_px(SIZE_PALETTE * cos(angle), SIZE_PALETTE * sin(angle))
		EngineObjectSegment.__init__(self,
			engine			= engine,
			colltype		= COLLTYPE_ROBOT,
			posA			= tuple(pa),
			posB			= tuple(pb),
			color			= "orange",
			mass			= 10,
			width			= WIDTH_PALETTE
		)

class Balais(EngineObjectPoly):
	def __init__(self, engine, robot, angle_balais):
		angle = robot.angle() + radians(angle_balais)
		points = [(0,0), (SIZE_BALAIS,0), (SIZE_BALAIS,WIDTH_BALAIS), (0,WIDTH_BALAIS)]
		points = map(lambda p: Vec(mm_to_px(p)), points)
		cosa = cos(angle)
		sina = sin(angle)
		points = map(lambda v: ((v[0]*cosa - sina*v[1]), (v[0]*sina + v[1]*cosa)), points)

		d = mm_to_px(WIDTH_MINI/2+ECART_ROBOT_BALAIS+SIZE_BALAIS/2)
		offset = (d*cosa, d*sina)
		EngineObjectPoly.__init__(self,
			engine			= engine,
			colltype		= COLLTYPE_ROBOT,
			offset 			= offset,
			poly_points		= list(points),
			color			= "orange",
			mass			= MASS_INF,
		)


class MiniRobot(Robot):
	def __init__(self, *, engine, canal_asserv, canal_others, posinit, team, match):
		Robot.__init__(self,
			engine 				= engine,
			canal_asserv		= canal_asserv,
			canal_others		= canal_others,
			team				= team,
			posinit				= posinit,
			mass				= 10,
			typerobot			= MINI,
			poly_points			= mm_to_px((0,0),(HEIGHT_MINI,0),(HEIGHT_MINI,WIDTH_MINI),(0,WIDTH_MINI)),
		)

		self.match = match
		
		"""self.palette_left = Palette(engine, mm_to_px(HEIGHT_MINI/2, WIDTH_MINI/2), 45)
		self.palette_right = Palette(engine, mm_to_px(HEIGHT_MINI/2, -WIDTH_MINI/2), -45)"""
		self.balais_left = Balais(engine, self, 90)
		self.balais_right = Balais(engine, self, -90)

		self.state_balais_left	= False
		self.state_balais_right	= False
		


	def remove_balais_left(self):
		print("left")
		self.remove_body_extension(self.balais_left)

	def add_balais_left(self):
		self.add_body_extension(self.balais_left)

	def remove_balais_right(self):
		print("right")
		self.remove_body_extension(self.balais_right)

	def add_balais_right(self):
		self.add_body_extension(self.balais_right)
	
	def _cmd_others_balais(self, right, on):
		if right:
			if on:
				self.add_balais_right()
			else:
				self.remove_balais_right()
		else:
			if on:
				self.add_balais_left()
			else:
				self.remove_balais_left()

	def onEvent(self, event):
		if not Robot.onEvent(self, event):
			if self._event_concerns_me(event):
				if KEYDOWN == event.type:
					if KEY_ARRACHE == event.key:
						self._cmd_others_arrachercarte(id_msg=42)
						return True
					elif KEY_BALAIS_LEFT == event.key:
						self.state_balais_left = not self.state_balais_left
						self._cmd_others_balais(LEFT, self.state_balais_left)
						return True
					elif KEY_BALAIS_RIGHT == event.key:
						self.state_balais_right = not self.state_balais_right
						self._cmd_others_balais(RIGHT, self.state_balais_right)
						return True

	def _cmd_others_arrachercarte(self, **kwargs):
		self.match.arracher_carte(self.team)
		self.send_canal_others(kwargs['id_msg'], 1)
