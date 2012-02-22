
from geometry.vec import *



class Action:
	def __init__(self, point_acces, gamestate, f):
		self.point_acces = Vec(point_acces)
		self.gamestate = gamestate
		self.score = 0
		self.f = f

	def dist_from(self, p):
		""" retourne la distance pour se rendre au point d'entrée
		de cette action """
		return (self.point_acces - p).norm2()

	def compute_score(self, p):
		"""
		Calculer le score de cette action
		@param p position de départ
		"""
		dist = self.dist_from(p)
		dist_enemies = ( self.dist_from(r.pos) for r in self.gamestate.enemyrobots() )
		self.score = dist + max(0, 2000 - min(dist_enemies))
		
	def __repr__(self):
		return "Action(%s)" % (self.point_acces)
		


