# -*- coding: utf-8 -*-

SEP		= '.'

FPS					= 60
PX_TO_MM			= 4

CIRCLE				= 0
POLY				= 1
WALL				= 2

COLLTYPE_DEFAULT	= 0
COLLTYPE_WALL		= 1
COLLTYPE_ROBOT		= 2
COLLTYPE_CD			= 3
COLLTYPE_ROULEAU	= 4


BLUE				= 0
RED					= 1

T_CD				= 0
T_LINGOT			= 1

class EngineObject:
	def __init__(self, *custom_objects):
		self.custom_objects = list(custom_objects)
		self.offset = (0,0)
	
	def step(self, dt):
		pass


def mm_to_px(*args):
	"""
	@param args une liste ou un atom de valeurs ou de positions
	@param la veleur ou la liste convertie

	>>> mm_to_px(4) == int(4/PX_TO_MM)
	True

	>>> mm_to_px((4,8)) == (mm_to_px(4),mm_to_px(8))
	True

	>>> mm_to_px((4,8),(16,32)) == ((mm_to_px(4),mm_to_px(8)), (mm_to_px(16),mm_to_px(32)))
	True

	>>> mm_to_px([(4,8),(16,32)]) == mm_to_px((4,8),(16,32))
	True

	>>> mm_to_px(*[(4,8),(16,32)]) == mm_to_px((4,8),(16,32))
	True
	
	"""
	def f(a):
		if type(a) == tuple or type(a) == list:
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
		if type(a) == tuple or type(a) == list:
			return px_to_mm(*a)
		else:
			return int(a * PX_TO_MM)
	if len(args) == 1:
		return f(args[0])
	else:
		return tuple(map(lambda v: f(v), args))

#####
### IRC
#####

SUFFIX_MINI			= "mini"
CANAL_ASSERV		= "#asserv"


if __name__ == "__main__":
	import doctest
	doctest.testmod()



