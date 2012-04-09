

from .action import *

from .define import *

class ActionTotem(Action):

	DIRECTION_HAUT		= 0
	DIRECTION_BAS		= 1
	
	def __init__(self, robot, asservissement, enemies, point_acces, direction):
		Action.__init__(self, robot, enemies, point_acces)
		self.direction = direction

	def action(self):
		print("YOUHOU")


class ActionBouteille(Action):
	def __init__(self, robot, asservissement, enemies, point_acces):
		Action.__init__(self, robot, enemies, point_acces)

	def action(self):
		print("YOUHOU")
		
class ActionCarte(Action):
	def __init__(self, robot, enemies, point_acces):
		Action.__init__(self, robot, enemies, point_acces)

	def action(self):
		print("YOUHOU")
		


def get_actions_bigrobot(robot, asservissement, enemies):
	actions = []
	# totems
	actions.append(ActionTotem(robot, asservissement, enemies, (1100,1125+R_BIGROBOT+1), ActionTotem.DIRECTION_BAS))
	actions.append(ActionTotem(robot, asservissement, enemies, (1900,1125+R_BIGROBOT+1), ActionTotem.DIRECTION_BAS))
	actions.append(ActionTotem(robot, asservissement, enemies, (1100,875-R_BIGROBOT-1), ActionTotem.DIRECTION_HAUT))
	actions.append(ActionTotem(robot, asservissement, enemies, (1900,875-R_BIGROBOT-1), ActionTotem.DIRECTION_HAUT))


	return actions


	

def get_actions_minirobot(robot, asservissement, enemies):
	actions = []
	# bouteilles
	actions.append(ActionBouteille(robot, asservissement, enemies, (640, 2000 - R_MINIROBOT - 10)))
	actions.append(ActionBouteille(robot, asservissement, enemies, (1883, 2000 - R_MINIROBOT - 10)))
	actions.append(ActionCarte(robot, enemies, (1500, R_MINIROBOT + 10)))

	return actions
