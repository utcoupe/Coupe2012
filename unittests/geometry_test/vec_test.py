

import unittest


import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__),".."))
from geometry import Vec


class VecInitTestCase(unittest.TestCase):

	def test_init_from_list(self):
		v = Vec([1,2,3])
		self.assertIsInstance(v, Vec)
		
	def test_init_from_tuple(self):
		v = Vec((1,2,3))
		self.assertIsInstance(v, Vec)
		
	def test_init_from_iterator(self):
		v = Vec(range(4))
		self.assertIsInstance(v, Vec)

class VecAddTestCase(unittest.TestCase):
	def setUp(self):
		self.v1 = Vec([1,2,3])
		self.v2 = Vec([4,5,6])
	
	def test_add(self):
		v = self.v1 + self.v2
		self.assertEqual(v, (5,7,9))

	def test_add_iterable(self):
		v = (4,5,6) + self.v1
		self.assertEqual(v, (5,7,9))
		
	def test_add_iterable2(self):
		v = self.v1 + (4,5,6)
		self.assertEqual(v, (5,7,9))

	def test_iadd(self):
		self.v1 += self.v2
		self.assertEqual(self.v1, (5,7,9))



class VecSubTestCase(unittest.TestCase):
	def setUp(self):
		self.v1 = Vec([1,2,3])
		self.v2 = Vec([4,5,6])
		
	def test_sub(self):
		v = self.v1 - self.v2
		self.assertEqual(v, (-3,-3,-3))

	def test_sub_iterable(self):
		v = (1,2,3) - self.v2
		self.assertEqual(v, (-3,-3,-3))
		
	def test_sub_iterable2(self):
		v = self.v1 - (4,5,6)
		self.assertEqual(v, (-3,-3,-3))

	def test_isub(self):
		self.v1 -= self.v2
		self.assertEqual(self.v1, (-3,-3,-3))



class VecMulTestCase(unittest.TestCase):
	def setUp(self):
		self.v = Vec([1,2,3])

	def test_mul(self):
		v = self.v * 3
		self.assertEqual(v, (3,6,9))
		
	def test_mul2(self):
		v = 3 * self.v
		self.assertEqual(v, (3,6,9))

	def test_imul(self):
		self.v *= 3
		self.assertEqual(self.v, (3,6,9))


class VecDivTestCase(unittest.TestCase):
	def setUp(self):
		self.v = Vec([4,5,6])

	def test_div(self):
		v = self.v / 2
		self.assertEqual(v, (2.0,2.5,3.0))

	def test_idiv(self):
		self.v /= 2
		self.assertEqual(self.v, (2.0,2.5,3.0))

class VecRoundTestCase(unittest.TestCase):
	def test_round(self):
		v = Vec([4.2,5.6,6.0])
		self.assertEqual(v.round(), (4,6,6))


class VecNormTestCase(unittest.TestCase):
	def setUp(self):
		self.v = Vec((5,5))

	def test_norm2(self):
		self.assertEqual(self.v.norm2(), 50)

	def test_norm(self):
		self.assertEqual(round(self.v.norm()), 7)
