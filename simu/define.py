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
	"""
	@param args une liste ou un atom de valeurs ou de positions
	@param la veleur ou la liste convertie
	"""
	def f(a):
		if type(a) == tuple:
			return mm_to_px(*a)
		else:
			return int(a / PX_TO_MM)
	if len(args) == 1:
		return f(args[0])
	else:
		return tuple(map(lambda v: f(v), args))

def px_to_mm(*args):
	"""
	@param args une liste ou un atom de valeurs ou de positions
	@param la veleur ou la liste convertie
	"""
	def f(a):
		if type(a) == tuple:
			return mm_to_px(*a)
		else:
			return int(a * PX_TO_MM)
	if len(args) == 1:
		return f(args[0])
	else:
		return tuple(map(lambda v: f(v), args))


CANAL_ASSERV		= "#asserv"
