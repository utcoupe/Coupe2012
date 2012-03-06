

import unittest



import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__),".."))
from geometry import AABB



class AABBTestCase(unittest.TestCase):
	def setUp(self):
		self.box = AABB((0,0), 3, 4)

	def test_contain(self):
		self.assertNotIn((-1,2), self.box)
		self.assertNotIn((5,2), self.box)
		self.assertNotIn((2,-1), self.box)
		self.assertNotIn((2,5), self.box)
		self.assertIn((2,2), self.box)
		self.assertIn((0,0), self.box)
		self.assertIn((2,3), self.box)
		self.assertNotIn((3,4), self.box)

	def test_aire(self):
		self.assertEqual(self.box.aire(), 12)

