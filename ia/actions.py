

from action import *

class ActionTotem(Action):

	DIRECTION_HAUT		= 0
	DIRECTION_BAS		= 1
	
	def __init__(self, gamestate, robot, point_acces, direction):
		Action.__init__(self, point_acces, gamestate, self.action)
		self.robot = robot
		self.direction = direction

	def action(self):
		print("YOUHOU")
		



def get_actions_bigrobot(gamestate, robot):
	actions = []
	#totems
	actions.append(ActionTotem(gamestate, robot, (1100,1350), ActionTotem.DIRECTION_BAS))
	actions.append(ActionTotem(gamestate, robot, (1900,1350), ActionTotem.DIRECTION_BAS))
	actions.append(ActionTotem(gamestate, robot, (1100,650), ActionTotem.DIRECTION_HAUT))
	actions.append(ActionTotem(gamestate, robot, (1900,650), ActionTotem.DIRECTION_HAUT))


	return actions


	
