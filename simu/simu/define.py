# -*- coding: utf-8 -*-

from pymunk import inf as MASS_INF
from pygame.locals import *



FPS					= 50
PX_TO_MM			= 4



COLLTYPE_DEFAULT	= 0
COLLTYPE_WALL		= 1
COLLTYPE_ROBOT		= 2
COLLTYPE_CD			= 3
COLLTYPE_ROULEAU	= 4
COLLTYPE_LINGO		= 5


BLUE				= 0
RED					= 1

T_CD				= 0
T_LINGO				= 1
T_CARTE				= 2
T_BOTTLE			= 3

COEFF_ENGORGEMENT_CD	= 0.05 # eq : on peut mettre 20 Cds avant d'être plein (0.05 * 20 = 1)
COEFF_ENGORGEMENT_LINGO	= 0.2



KEY_CHANGE_TEAM		= K_LSHIFT		# changer de robot
KEY_CHANGE_ROBOT	= K_LCTRL		# changer d'équipe
KEY_STOP_RESUME		= K_SPACE		# apppui = stop, relache = resume
KEY_CANCEL			= K_ESCAPE		# cancel
KEY_DROP			= K_d			# vider le gros robot
KEY_ARRACHE			= K_a			# arracher la carte
KEY_BALAIS_LEFT		= K_q			# sortir le balais de gauche
KEY_BALAIS_RIGHT	= K_w			# sortir le balais de droite
KEY_BULDO			= K_x			# mode buldozer
KEY_TELEPORTATION	= K_t			# mode téléportation


BIG			= 0
MINI		= 1

LEFT		= 0
RIGHT		= 1

# dimensions du petit robot
WIDTH_MINI 		= 184
HEIGHT_MINI 	= 200
# dimension des palettes (buldo) du mini robot
SIZE_PALETTE	= 70
WIDTH_PALETTE	= 5
# dimensions balais qui râclent la zone adverse	
SIZE_BALAIS		= 180
WIDTH_BALAIS	= 50
ECART_ROBOT_BALAIS	= 0

#ECART_CENTRE = -65 # ecart par rapport au centre du robot

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
	@param la valeur ou la liste convertie
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




if __name__ == "__main__":
	import doctest
	doctest.testmod()



