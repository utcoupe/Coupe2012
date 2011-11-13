# -*- coding: utf-8 -*-

from define import *
from motorphysic import *
from motorgraphic import *

class Engine:
	"""
	Engine, cette classe permet de coupler un moteur physique et un
	moteur graphique.
	"""
	def __init__(self, stop_irc, match):
		"""
		@param stop_irc fonction permettant d'arreter le client irc (il
		tourne dans un thread à part).
		"""
		self.match = match
		self.motorgraphic = MotorGraphic()
		self.motorphysic = MotorPhysic()
		self.motorphysic.add_collision_handler(COLLTYPE_ROBOT, COLLTYPE_WALL, self.motorgraphic.draw_collision)
		self.motorphysic.add_collision_handler(COLLTYPE_ROULEAU, COLLTYPE_CD, self.on_collision_rouleau_cd)
		self.running = True
		self.objects = []
		self.stop_irc = stop_irc
		self.objects_to_remove = []

	def find_obj_by_shape(self, shape):
		"""
		À partir d'une shape retrouve l'objet concerné
		"""
		objs = list(filter(lambda obj: obj.shape == shape, self.objects))
		if len(objs) > 1:
			raise Exception("shape %s is used multiple times : %s" % (shape, objs))
		elif len(objs) == 1:
			return objs[0]
		else:
			return None

	def find_robot_by_rouleau_shape(self, shape):
		"""
		À retrouve le robot dont le rouleau à la shape précisée
		"""
		objs = list(filter(lambda obj: "rouleau" in dir(obj) and obj.rouleau.shape == shape, self.objects))
		if len(objs) > 1:
			raise Exception("shape %s is used multiple times : %s" % (shape, objs))
		elif len(objs) == 1:
			return objs[0]
		else:
			return None
	
	def on_collision_rouleau_cd(self, space, arb):
		"""
		Quand un rouleau touche un cd
		"""
		robot = self.find_robot_by_rouleau_shape(arb.shapes[0])
		if not robot:
			print("robot not found")
		else:
			team = robot.team
			self.match.add_points(team, T_CD)
			print(self.match.score)
			cd = self.find_obj_by_shape(arb.shapes[1])
			self.objects_to_remove.append(cd)

	def add(self, obj):
		"""
		Ajouter un objet à l'engine, il est ajouté du même coup au moteur
		physique et au moteur graphique.
		"""
		self.objects.append(obj)
		self.motorgraphic.add(obj)
		self.motorphysic.add(obj)

	def start(self):
		"""
		Démarrer l'engine
		"""
		while self.running:
			self.step()
		self.stop_irc()

	def step(self):
		"""
		Effectuer un step
		"""
		for o in self.objects_to_remove:
			if o: self.remove(o)
		self.objects_to_remove = []
		dt = 1.0/float(FPS)
		self.motorphysic.step(dt)
		for o in self.objects:
			o.step(dt)
		self.running = self.motorgraphic.step()

	def remove(self, obj):
		self.motorgraphic.remove(obj)
		self.motorphysic.remove(obj)
		self.objects.remove(obj)
		
