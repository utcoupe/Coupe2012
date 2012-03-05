
import unittest

from ia.graph.pathfinding.node import *




class NodeTestCase(unittest.TestCase):
	def setUp(self):
		self.n = Node()
		self.n2 = Node()
		self.n.init(3,(3,4),[self.n2])
		
	def test_id(self):
		self.assertEqual(self.n.id, 3)

	def test_pos(self):
		self.assertEqual(self.n.pos, (3,4))

	def test_neighbors(self):
		self.assertEqual(self.n.neighbors, [self.n2])


class NodeResetTestCase(unittest.TestCase):
	def setUp(self):
		self.n = Node()
		self.n2 = Node()
		self.n.init(3,(3,4),[self.n2])
		self.n.reset()

	def test_P(self):
		self.assertEqual(self.n.P, 0)

	def test_H(self):
		self.assertEqual(self.n.H, 0)

	def test_visited(self):
		self.assertFalse(self.n.visited, 0)

	def test_previous_in_path(self):
		self.assertIsNone(self.n.previous_in_path)
	

class NodeLiaisonsTestCase(unittest.TestCase):
	def setUp(self):
		self.n = Node()
		self.n2 = Node()
		self.n.init(3,(3,4),[self.n2])
		
	def test_liaison(self):
		self.assertEqual(self.n.liaison(self.n2), 5.0)
		
	def test_heuristique(self):
		self.n.calc_heuristique(self.n2)
		self.assertEqual(self.n.H, 7)
