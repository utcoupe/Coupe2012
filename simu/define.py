# -*- coding: utf-8 -*-

CIRCLE				= 0
POLY				= 1

COLLTYPE_DEFAULT	= 0


class EngineObject:
	def step(self, dt):
		pass


def mm_to_px(pos):
	x,y = pos
	return x/4.0, y/4.0

def px_to_mm(pos):
	x,y = pos
	return x*4.0,y*4.0

