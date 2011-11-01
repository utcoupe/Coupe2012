# -*- coding: utf-8 -*-

CIRCLE				= 0
POLY				= 1
WALL				= 2

COLLTYPE_DEFAULT	= 0


class EngineObject:
	def step(self, dt):
		pass


def mm_to_px(pos):
	if type(pos) == tuple:
		x,y = pos
		return int(x/4.0), int(y/4.0)
	else:
		return int(pos/4.0)

def px_to_mm(pos):
	if type(pos) == tuple:
		x,y = pos
		return int(x*4.0), int(y*4.0)
	else:
		return int(pos*4.0)


