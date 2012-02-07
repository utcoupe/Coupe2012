#!/usr/bin/python3
# -*- coding: utf-8 -*-


from vec import Vec


class Segment:
	def __init__(self, a, b):
		self.a = Vec(a)
		self.b = Vec(b)

	def pos_point(self, p):
		"""
			>0 : à gauche
			<0 : à droite
			=0 : aligné
		"""
		ab = self.b - self.a
		ap = Vec(p) - self.a
		return ab[0] * ap[1] - ab[1] * ap[0]

	def __repr__(self):
		return "Segment(%s, %s)" % (self.a, self.b)


import doctest
doctest.testfile("doctest/segment.txt")
