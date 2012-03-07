# -*- coding: utf-8 -*-

from .define import *
from geometry import ConvexPoly

points = {
	T_CD: 1,
	T_LINGO: 3,
	T_CARTE: 5,
	T_BOTTLE: 5,
}

class Match:
	def __init__(self):
		self.carte_arrache = [0,0]

	def init(self, engine):
		self.engine = engine

	def arracher_carte(self, team):
		self.carte_arrache[team] = 1

	def score(self, team):
		cds, lingos = self.get_cds_lingos(team)
		score  = self.carte_arrache[team] * points[T_CARTE]
		score += cds * points[T_CD]
		score += lingos * points[T_LINGO]
		return score

	def get_cds_lingos(self, team):
		if team == BLUE:
			return self.get_cds_lingos_for_blue()
		elif team == RED:
			return self.get_cds_lingos_for_red()
		else:
			raise Exception("team inconnue")
	
	def get_cds_lingos_for_blue(self):
		poly = ConvexPoly(mm_to_px(
			(0,500),
			(400,500),
			(325,2000),
			(0,2000)
		))
		poly2 = ConvexPoly(mm_to_px(
			(0,0),
			(500,0),
			(500,500),
			(0,500)
		))
		return self._get_cds_lingos_in_poly(poly) + self._get_cds_lingos_in_poly(poly2)
		
	def get_cds_lingos_for_red(self):
		poly = ConvexPoly(mm_to_px(
			(3000,500),
			(2600,500),
			(2675,2000),
			(3000,2000)
		))
		poly2 = ConvexPoly(mm_to_px(
			(2500,0),
			(3000,0),
			(3000,500),
			(2500,500)
		))
		return self._get_cds_lingos_in_poly(poly) + self._get_cds_lingos_in_poly(poly2)

	def _get_cds_lingos_in_poly(self, poly):
		cds = []
		for obj in self.engine.objects:
			if isinstance(obj, Cd):
				if cd.pos() in poly:
					cds.append(cd)
			if isinstance(obj, Lingo):
				if lingo.pos() in poly:
					lingos.append(obj)
		return cds,lingos
		



	
