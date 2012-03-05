


import unittest


from ia.graph.nav.funnel import *
from geometry import ConvexPoly




class FunnelTestCase(unittest.TestCase):
	def setUp(self):
		self.d = (0,0)
		self.a = (4,0)
		self.e1 = ((1,-1),(1,1))
		self.e2 = ((2,-1),(2,1))
		self.e3 = ((3,-1),(3,1))
		
	def test_funnel1(self):
		self.assertEqual(funnel(self.d, self.a, [self.e1,self.e2,self.e3]), [self.d,self.a])

	def test_funnel2(self):
		e = ((1.5,7),(1.5,8))
		self.assertEqual(funnel(self.d, self.a, [self.e1,e,self.e2,self.e3]), [self.d, (1,1), (1.5,7), (2,1), self.a])

class FunnelUtilTestCase(unittest.TestCase):
	def test_polys_to_portal_edges(self):
		d = (0,0)
		a = (4,0)
		n1 = ConvexPoly(((-1,0),(1,-1),(1,1)))
		n2 = ConvexPoly(((1,-1),(2,-1),(1,1)))
		n3 = ConvexPoly(((1,1),(2,-1),(2,1)))
		n4 = ConvexPoly(((2,-1),(3,-1),(3,1),(2,1)))
		n5 = ConvexPoly(((3,1),(3,-1),(5,0)))
		
		portal_edges = polys_to_portal_edges((n1,n2,n3,n4,n5))
		self.assertEqual(portal_edges, [((1, -1), (1, 1)), ((2, -1), (1, 1)), ((2, -1), (2, 1)), ((3, -1), (3, 1))])

		portal_edges = funnel(d, a, portal_edges)
		self.assertEqual(portal_edges, [d,a])
		



