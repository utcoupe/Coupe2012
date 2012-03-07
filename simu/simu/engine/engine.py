# -*- coding: utf-8 -*-


import threading



from ..define import *
from .motorphysic import MotorPhysic
from .motorgraphic import MotorGraphic





class Engine:
	"""
	Engine, cette classe permet de coupler un moteur physique et un
	moteur graphique.
	"""
	def __init__(self, debug):
		"""
		@param stop_irc fonction permettant d'arreter le client irc (il
		tourne dans un thread à part).
		"""
		self.motorgraphic = MotorGraphic(debug)
		self.motorphysic = MotorPhysic()
		self.motorphysic.add_collision_handler(COLLTYPE_ROBOT, COLLTYPE_WALL, self.motorgraphic.draw_collision)
		self.motorphysic.add_collision_handler(COLLTYPE_ROULEAU, COLLTYPE_CD, self.on_collision_rouleau_cd)
		self.motorphysic.add_collision_handler(COLLTYPE_ROULEAU, COLLTYPE_LINGO, self.on_collision_rouleau_lingo)
		self.e_stop = threading.Event()
		self.objects = []
		self.objects_to_remove = []

	def init(self, stop_irc, match):
		self.match = match
		self.stop_irc = stop_irc

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
			cd = self.find_obj_by_shape(arb.shapes[1])
			if not cd:
				print("Cd not found")
			else:
				robot.eat_cd(cd.color)
				self.objects_to_remove.append(cd)

	def on_collision_rouleau_lingo(self, space, arb):
		"""
		Quand un rouleau touche un cd
		"""
		robot = self.find_robot_by_rouleau_shape(arb.shapes[0])
		if not robot:
			print("robot not found")
		else:
			lingo = self.find_obj_by_shape(arb.shapes[1])
			if not lingo:
				print("Lingo not found")
			else:
				robot.eat_lingo()
				self.objects_to_remove.append(lingo)
		

	def add(self, obj):
		"""
		Ajouter un objet à l'engine, il est ajouté du même coup au moteur
		physique et au moteur graphique.
		"""
		self.objects.append(obj)
		self.motorgraphic.add(obj)
		self.motorphysic.add(obj)

	def stop(self):
		self.e_stop.set()
	
	def start(self):
		"""
		Démarrer l'engine
		"""
		while not self.e_stop.is_set():
			try:
				self.step()
			except KeyboardInterrupt as ex:
				print("Exit")
				break
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
		if not self.motorgraphic.step():
			self.stop()

	def remove(self, obj):
		self.motorgraphic.remove(obj)
		self.motorphysic.remove(obj)
		self.objects.remove(obj)
		
