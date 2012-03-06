
import unittest

import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__),".."))
from geometry import Poly



class PolyAABBTestCase(unittest.TestCase):
	def setUp(self):
		self.poly = Poly(((0,0),(4,0),(2,2)))

	def test_pos(self):
		self.assertEqual(self.poly.AABB.p, (0,0))

	def test_width(self):
		self.assertEqual(self.poly.AABB.w, 4)

	def test_height(self):
		self.assertEqual(self.poly.AABB.h, 2)


class PolyAdjacentTestCase(unittest.TestCase):
	def setUp(self):
		self.poly = Poly(((0,0),(4,0),(2,2)))

	def test_adjacent1(self):
		poly1 = Poly(((0,0),(4,0),(2,2)))
		poly2 = Poly(((0,0),(2,-2),(4,0)))
		self.assertTrue(poly1.adjacent(poly2))

	def test_adjacent2(self):
		poly1 = Poly(((0,0),(4,0),(2,2)))
		poly2 = Poly(((0,1),(4,1),(2,2)))
		self.assertFalse(poly1.adjacent(poly2))
		
class PolyMoveTestCase(unittest.TestCase):
	def test_move(self):
		poly = Poly(((0,0),(4,0),(2,2)))
		_ = poly.move((2,2)).round()
		self.assertEqual(poly.points, [(2,2), (6,2), (4,4)])
		
	def test_move_to(self):
		poly = Poly(((0,0),(4,0),(4,4),(0,4)))
		_ = poly.move_to((0,0)).round()
		self.assertEqual(poly.points, [(-2, -2), (2, -2), (2, 2), (-2, 2)])

	
