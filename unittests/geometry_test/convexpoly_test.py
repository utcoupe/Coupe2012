
import unittest

import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__),".."))
from geometry import ConvexPoly



class ConvexPolyTestCase(unittest.TestCase):
	def setUp(self):
		self.poly = ConvexPoly(((0,0),(4,0),(2,2)))

	def test_contain(self):
		self.assertIn((2,1), self.poly)

	def test_contain(self):
		self.assertNotIn((-1,4), self.poly)

	def test_clockwise(self):
		self.assertFalse(self.poly.is_clockwise())

	def test_intersect(self):
		from geometry import Segment
		s = Segment((-1,-1), (-3,-3))
		self.assertFalse(self.poly.intersect(s))
		s = Segment((-1,-1), (5,5))
		self.assertTrue(self.poly.intersect(s))

