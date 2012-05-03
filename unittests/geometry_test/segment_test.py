
import unittest


import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__),".."))
from geometry import Segment, Vec


class SegmentTestCase(unittest.TestCase):
	def setUp(self):
		self.a = (0,0)
		self.b = (4,0)
		self.s = Segment(self.a,self.b)
	
	def test_init(self):
		s = self.s
		self.assertIsInstance(s.a, Vec)
		self.assertIsInstance(s.b, Vec)
		self.assertEqual(s.a, self.a)
		self.assertEqual(s.b, self.b)

	def test_equal(self):
		self.assertEqual(self.s, (self.a, self.b))

	def test_getitem(self):
		self.assertEqual(self.s[0], self.a)
		self.assertEqual(self.s[1], self.b)
		
	def test_iter(self):
		t = tuple(p for p in self.s)
		self.assertEqual(t, self.s)
		self.assertEqual(self.s, t)

	def test_pos_point(self):
		self.assertEqual(self.s.pos_point((2,0)),0)
		self.assertTrue(self.s.pos_point((2,2)) > 0)
		self.assertTrue(self.s.pos_point((2,-2)) < 0)


class SegmentProjectionTest(unittest.TestCase):
	def setUp(self):
		self.s1 = Segment((0,0), (4,0))
		self.s2 = Segment((0,0),(0,-5))
	
	def test_dist_from_point(self):
		self.assertEqual(self.s1.dist_from_point((0,2)), 2)
		self.assertEqual(self.s1.dist_from_point((2,0)), 0)
		self.assertEqual(self.s1.dist_from_point((300,-5)), 5)
		self.assertEqual(self.s2.dist_from_point((0,2)), 0)
		self.assertEqual(self.s2.dist_from_point((2,0)), 2)
		self.assertEqual(self.s2.dist_from_point((-300,-5)), 300)
	
	def test_orthogonal_projection(self):
		self.assertEqual(self.s1.orthogonal_projection((0,2)), (0,0))
		self.assertEqual(self.s1.orthogonal_projection((2,0)), (2,0))
		self.assertEqual(self.s1.orthogonal_projection((-5,3)), (-5,0))
		self.assertEqual(self.s2.orthogonal_projection((0,2)), (0,2))
		self.assertEqual(self.s2.orthogonal_projection((2,0)), (0,0))
		self.assertEqual(self.s2.orthogonal_projection((-5,3)), (0,3))

class SegmentIntersectTestCase(unittest.TestCase):
	def test_intersect1(self):
		s1 = Segment((0,0), (4,0))
		s2 = Segment((-1,-1),(5,1))
		self.assertTrue(s1.intersect(s2))


	def test_intersect2(self):
		s1 = Segment((0,0), (4,0))
		s2 = Segment((-1,-1),(5,0))
		self.assertFalse(s1.intersect(s2))


	def test_intersect3(self):
		s1 = Segment((2, 2), (0, 0))
		s2 = Segment((-1, -1), (-3, -3))
		self.assertFalse(s1.intersect(s2))


