



import unittest

from ia.graph.pathfinding.a_star import *
from ia.graph.pathfinding.node import *



class AStarTestCase(unittest.TestCase):
	def setUp(self):
		self.n1 = Node()
		self.n2 = Node()
		self.n3 = Node()
		self.n4 = Node()
		self.n1.init(1, (0,0), (self.n2,self.n3))
		self.n2.init(2, (100,100), (self.n1,self.n4))
		self.n3.init(3, (1,1), (self.n1,self.n4))
		self.n4.init(4, (2,2), (self.n2,self.n3))
		self.p = AStar((self.n1,self.n2,self.n3,self.n4), NAVGRAPH)
		
	def test_astar_pathfinding1(self):
		nodes,raw_path = self.p.compute_path(self.n1,self.n4)
		self.assertEqual(nodes, [self.n1,self.n3,self.n4])

	def test_astar_pathfinding1(self):
		self.n1.neighbors = ()
		nodes,raw_path = self.p.compute_path(self.n1,self.n4)
		self.assertEqual(nodes, [])
		









	
