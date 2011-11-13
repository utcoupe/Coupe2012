# -*- coding: utf-8 -*-

from define import *
from motorphysic import *
from motorgraphic import *

class Engine:
	"""
	Engine, cette classe permet de coupler un moteur physique et un
	moteur graphique.
	"""
	def __init__(self, stop_irc):
		"""
		@param stop_irc fonction permettant d'arreter le client irc (il
		tourne dans un thread à part).
		"""
		self.motorgraphic = MotorGraphic()
		self.motorphysic = MotorPhysic()
		self.motorphysic.add_collision_handler(COLLTYPE_ROBOT, COLLTYPE_WALL, self.motorgraphic.draw_collision)
		self.motorphysic.add_collision_handler(COLLTYPE_ROULEAU, COLLTYPE_CD, self.on_collision_rouleau_cd)
		self.running = True
		self.objects = []
		self.stop_irc = stop_irc
		self.objects_to_remove = []

	def on_collision_rouleau_cd(self, space, arb):
		cd_to_remove = filter(lambda obj: obj.shape == arb.shapes[1], self.objects)
		self.objects_to_remove += list(cd_to_remove)

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
			self.remove(o)
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
		
