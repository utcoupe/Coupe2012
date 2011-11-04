# -*- coding: utf-8 -*-

FPS					= 60
PX_TO_MM			= 4

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


def mm_to_px(*args):
	if len(args) == 1:
		return int(args[0] / PX_TO_MM)
	else:
		return tuple(map(lambda v: int(v / PX_TO_MM), args))

def px_to_mm(*args):
	if len(args) == 1:
		return int(args[0] * PX_TO_MM)
	else:
		return tuple(map(lambda v: int(v * PX_TO_MM), args))


CANAL_ASSERV		= "#asserv"
