# -*- coding: utf-8 -*-

from .define import *


points = {
	T_CD: 1,
	T_LINGOT: 3
}

class Match:
	def __init__(self):
		self.score = [0,0]

	def reset(self):
		self.score = [0,0]

	def add_points(self, team, obj_t):
		self.score[team] += points[obj_t]
