

from action import *



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
		



def get_actions_bigrobot(robot, asservissement, enemies):
	actions = []
	# totems
	actions.append(ActionTotem(robot, asservissement, enemies, (1100,1350), ActionTotem.DIRECTION_BAS))
	actions.append(ActionTotem(robot, asservissement, enemies, (1900,1350), ActionTotem.DIRECTION_BAS))
	actions.append(ActionTotem(robot, asservissement, enemies, (1100,650), ActionTotem.DIRECTION_HAUT))
	actions.append(ActionTotem(robot, asservissement, enemies, (1900,650), ActionTotem.DIRECTION_HAUT))


	return actions


	

def get_actions_minirobot(robot, asservissement, enemies):
	actions = []
	# bouteilles
	actions.append(ActionBouteille(robot, asservissement, enemies, (640, 1750)))
	actions.append(ActionBouteille(robot, asservissement, enemies, (1883, 1750)))

	return actions
