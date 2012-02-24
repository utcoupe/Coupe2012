
from geometry.vec import *


MAX_DIST = 1E50

class Action:
	
	def __init__(self, robot, enemies, point_acces):
		self.point_acces = Vec(point_acces)
		self.robot = robot
		self.enemies = enemies

		# calculé automatiquement par compute_score
		self.score = 0
		self.path = []

	def dist_from(self, p):
		""" retourne la distance pour se rendre au point d'entrée
		de cette action """
		return (self.point_acces - p).norm()

	def compute_path(self, p):
		_,_,self.path = self.robot.ng.get_path(p, self.point_acces)

	def get_len_path(self):
		if len(self.path) > 1:
			dist = 0
			last = self.path[0]
			for current in self.path[1:]:
				dist += (current - last).norm()
			return dist
		else:
			return MAX_DIST

	def compute_score(self, p):
		"""
		Calculer le score de cette action
		@param p position de départ
		"""
		self.compute_path(p)
		dist = self.get_len_path()
		dist_enemies = ( self.dist_from(r.pos) for r in self.enemies )
		min_dist_enemies = min(dist_enemies)
		self.score = dist - min_dist_enemies
		#print(self.point_acces, dist, min_dist_enemies, self.score)
	
	def __repr__(self):
		return "Action(%s, %s)" % (self.point_acces, self.score)
		


