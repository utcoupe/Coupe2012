
from .action import *

from .define import *

import time

class ActionTotem(Action):

	DIRECTION_HAUT		= 0	# il faudra aller vers le bas (dy < 0) pour aller vider le totem
	DIRECTION_BAS		= 1 # il faudra eller vers le haut (dy > 0) pour aller vider le totem
	
	def __init__(self, ia, robot, enemies, point_acces, direction):
		Action.__init__(self, ia, robot, enemies, point_acces)
		self.direction = direction

	def run(self):
		asserv = self.robot.asserv
		# tourner face au totem
		angle = 90 if self.direction==self.DIRECTION_HAUT else -90
		asserv.turn(angle, block=True, block_level=2)

		
		# avancer
		asserv.pwm(100,100,1000, block=True, block_level=2)

		# reculer
		point = Vec(self.point_acces)
		point[1] += -10 if self.direction==self.DIRECTION_HAUT else 10
		asserv.goto(point, block=True, block_level=2)
		
		#fini
		self.clean()

		print("YOUHOU le totem")
		
	def __repr__(self):
		return "ActionTotem(%s, %s)" % (self.point_acces, self.score)

class ActionTotemBas(Action):
	def __init__(self, ia, robot, enemies, point_acces):
		Action.__init__(self, ia, robot, enemies, point_acces)

	def run(self):
		print("\nACTION TOTEM ENCULEEEEE\n")

		self.robot.asserv.turn(self.ia.a(90), block=True, block_level=2)
		time.sleep(0.5)
		point = (1450, 1000-125-R_BIGROBOT+50)
		point2 = (400, 900)
		self.robot.asserv.goto(self.ia.p(point), block=True, block_level=2)
		self.robot.asserv.turn(self.ia.a(180), block=True, block_level=2)
		time.sleep(0.5)
		self.robot.actionneurs.tourner(0, 50)
		self.robot.actionneurs.tourner(2, -50)#, block=True, block_level=2)
		time.sleep(1)
		
		self.robot.asserv.gotor((300,0), block=True, block_level=2) # permet de vider le totem 
		self.robot.asserv.goto(self.ia.p(point2), block=True, block_level=2)
		self.robot.asserv.turn(self.ia.a(180), block=True, block_level=2)
		time.sleep(0.5)
		self.robot.asserv.pwm(100, 100, 700, block=True, block_level=2)

		self.robot.asserv.pwm(-100,-100, 700, block=True, block_level=2)
		self.robot.asserv.turn(self.ia.a(180), block=True, block_level=2)
		time.sleep(0.5)
		self.robot.actionneurs.ouvrir_peignes() # On protège les peignes
		
		#self.robot.actionneurs.tourner(0, -70)
		#self.robot.actionneurs.tourner(1, 70)
		self.clean()

	def compute_score(self, p):
		super().compute_score(p)
		#if len(self.robot.actions) > 3:
		#	self.score += 10000
		self.score = -MAX_DIST

class ActionTotemHaut(Action):
	def __init__(self, ia, robot, enemies, point_acces):
		Action.__init__(self, ia, robot, enemies, point_acces)

	def run(self):
		print("\nACTION TOTEM ENCULEEEEE\n")

		self.robot.asserv.turn(-90, block=True, block_level=2)
		time.sleep(0.5)
		self.robot.asserv.goto(self.ia.p((1450, 1000+125+R_BIGROBOT-30)))
		time.sleep(3)
		#self.robot.asserv.gotor((50,0), block=True,block_level=2) 
		self.robot.asserv.turn(self.ia.a(180), block=True, block_level=2)
		time.sleep(0.5)
		self.robot.actionneurs.tourner(0, 50)
		self.robot.actionneurs.tourner(2, -50)#, block=True, block_level=2)
		time.sleep(1)

		self.robot.asserv.gotor((300,0), block=True, block_level=2) # permet de vider le totem 
		self.robot.asserv.goto(self.ia.p((400, 900)), block=True, block_level=2)
		self.robot.asserv.turn(self.ia.a(180), block=True, block_level=2)
		self.robot.asserv.pwm(100, 100, 1200, block=True, block_level=2)

		self.robot.asserv.pwm(-100,-100, 1200, block=True, block_level=2)
		self.robot.actionneurs.ouvrir_peignes() # On protège les peignes

		self.clean()

	def compute_score(self, p):
		super().compute_score(p)
		self.score = MAX_DIST-2

class ActionFinalize(Action):
	"""
	Revenir à la zone de dépot et reculer un peu pour décharger les objects mangés par le robot
	"""

	def __init__(self, ia, robot, enemies, point_access):
		super().__init__(ia, robot, enemies, point_access)

	def run(self):
		ax12 = self.robot.actionneurs

		self.robot.asserv.turn(self.ia.a(180), block=True, block_level=2)

		self.robot.asserv.pwm(100, 100, 500, block=True, block_level=2)

		self.robot.asserv.pwm(-30, -30, 2000, block=True, block_level=2)
		time.sleep(80)
		self.clean()

	def compute_score(self, p):
		temps = time.time() - self.ia.t_begin_match
		if (temps > 70):
			super().compute_score(p)
			self.score = -MAX_DIST
		else:
			self.score = MAX_DIST

	def __repr__(self):
		return "ActionFinalize(%s, %s)" % (self.point_acces, self.score)


class ActionBouteille(Action):
	def __init__(self, ia, robot, enemies, point_acces):
		Action.__init__(self, ia, robot, enemies, point_acces)

	def run(self):
		print("\nACTION BOUTEILLE BIIIIITCH\n")

		self.robot.asserv.turn(-90, block=True, block_level=2)
		time.sleep(0.5)
		self.robot.asserv.pwm(-100,-100,1500, block=True, block_level=2)
		self.robot.asserv.pwm(100,100,1000, block=True, block_level=2)
		#self.robot.asserv.goto(self.point_acces)

		self.clean()

	def __repr__(self):
		return "ActionBouteille(%s, %s)" % (self.point_acces, self.score)

class ActionLingo(Action):
	"""
	Attrapper le ligno à côté de la zone de départ
	"""
	def __init__(self, ia, robot, enemies, point_acces):
		super().__init__(ia, robot, enemies, point_acces)

	def run(self):

		asserv = self.robot.asserv

		# tourne
		asserv.turn(self.ia.a(180), block=True, block_level=2)
		time.sleep(0.5)

		# avance
		asserv.pwm(100,100,700, block=True, block_level=2)

		# recule
		asserv.pwm(-100,-100,700, block=True, block_level=2)

		# retour au point de départ
		asserv.goto(self.point_acces, block=True, block_level=2)
		
		self.clean()
		print("LINGO POUR LES NOOBS LOL")

	def __repr__(self):
		return "ActionLingo(%s, %s)" % (self.point_acces, self.score)



def get_actions_bigrobot(ia, robot, enemies):
	actions = []

	actions.append(ActionBouteille(ia, robot, enemies, ia.p((640, 2000 - R_BIGROBOT - 100))))
	actions.append(ActionBouteille(ia, robot, enemies, (ia.x(1900), 2000 - R_BIGROBOT - 100)))
	#actions.append(ActionLingo(ia, robot, enemies, ia.p((400, 900))))
	actions.append(ActionTotemBas(ia, robot, enemies, ia.p((1450,1000-125-R_BIGROBOT-40))))
	actions.append(ActionTotemHaut(ia, robot, enemies, ia.p((1450,1000+125+R_BIGROBOT+40))))
	actions.append(ActionFinalize(ia, robot, enemies, ia.p((400, 950))))
	#actions.append(ActionTotem3(ia, robot, enemies, ia.p((2200,1000-125-R_BIGROBOT-60)), ActionTotem.DIRECTION_HAUT))
	#actions.append(ActionTotem3(ia, robot, enemies, ia.p((2200,1000+125+R_BIGROBOT+60)), ActionTotem.DIRECTION_BAS))

	return actions


	

def get_actions_minirobot(ia, robot, enemies):
	actions = []
	# bouteilles
	"""actions.append(ActionBouteille(robot, enemies, (640, 2000 - R_MINIROBOT - 10)))
	actions.append(ActionBouteille(robot, enemies, (1883, 2000 - R_MINIROBOT - 10)))
	actions.append(ActionCarte(robot, enemies, (1500, R_MINIROBOT + 10)))"""

	return actions
