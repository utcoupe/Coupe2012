

from .action import *

from .define import *

import time

class ActionTotem(Action):

	DIRECTION_HAUT		= 0	# il faudra aller vers le bas (dy < 0) pour aller vider le totem
	DIRECTION_BAS		= 1 # il faudra eller vers le haut (dy > 0) pour aller vider le totem
	
	def __init__(self, robot, enemies, point_acces, direction):
		Action.__init__(self, robot, enemies, point_acces)
		self.direction = direction

	def run(self):
		asserv = self.robot.asserv
		# tourner face au totem
		angle = 90 if self.direction==self.DIRECTION_HAUT else -90
		asserv.turn(angle, block=True, block_level=2)

		
		# avancer
		asserv.pwm(100,100)
		time.sleep(1)
		asserv.pwm(0,0)

		# reculer
		point = Vec(self.point_acces)
		point[1] += -10 if self.direction==self.DIRECTION_HAUT else 10
		asserv.goto(point, block=True, block_level=2)
		
		#fini
		self.clean()

		print("YOUHOU le totem")
		
	def __repr__(self):
		return "ActionTotem(%s, %s)" % (self.point_acces, self.score)
		

class ActionBouteille(Action):
	def __init__(self, robot, enemies, point_acces):
		Action.__init__(self, robot, enemies, point_acces)

	def run(self):
		print("YOUHOU")
		self.clean()

	def __repr__(self):
		return "ActionBouteille(%s, %s)" % (self.point_acces, self.score)

class ActionCarte(Action):
	def __init__(self, robot, enemies, point_acces):
		Action.__init__(self, robot, enemies, point_acces)
	
	def run(self):
		self.clean()
		print("YOUHOU")
		
	def __repr__(self):
		return "ActionCarte(%s, %s)" % (self.point_acces, self.score)


class ActionCd(Action):
	def __init__(self, robot, enemies, point_acces):
		super().__init__(robot, enemies, point_acces)

	def run(self):
		self.clean()
		print("YOUHOU le Cd")
	
	def compute_score(self, p):
		super().compute_score(p)
		self.score -= 1000

	def __repr__(self):
		return "ActionCd(%s, %s)" % (self.point_acces, self.score)

def get_actions_bigrobot(robot, enemies):
	actions = []
	# totems
	actions.append(ActionTotem(robot, enemies, (1100,1125+R_BIGROBOT+1), ActionTotem.DIRECTION_BAS))
	actions.append(ActionTotem(robot, enemies, (1900,1125+R_BIGROBOT+1), ActionTotem.DIRECTION_BAS))
	actions.append(ActionTotem(robot, enemies, (1100,875-R_BIGROBOT-1), ActionTotem.DIRECTION_HAUT))
	actions.append(ActionTotem(robot, enemies, (1900,875-R_BIGROBOT-1), ActionTotem.DIRECTION_HAUT))


	return actions


	

def get_actions_minirobot(robot, enemies):
	actions = []
	# bouteilles
	actions.append(ActionBouteille(robot, enemies, (640, 2000 - R_MINIROBOT - 10)))
	actions.append(ActionBouteille(robot, enemies, (1883, 2000 - R_MINIROBOT - 10)))
	actions.append(ActionCarte(robot, enemies, (1500, R_MINIROBOT + 10)))

	return actions
