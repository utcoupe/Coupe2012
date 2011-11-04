# -*- coding: utf-8 -*-

FPS					= 60


CIRCLE				= 0
POLY				= 1
WALL				= 2

COLLTYPE_DEFAULT	= 0
COLLTYPE_WALL		= 1
COLLTYPE_ROBOT		= 2
COLLTYPE_CD			= 3


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


CANAL_ASSERV		= "#asserv"
