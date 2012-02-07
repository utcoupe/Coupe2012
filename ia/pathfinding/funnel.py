

from geometry.segment import *


def polys_to_portal_edges(nodes):
	portal_edges = []
	for n1,n2 in zip(nodes[:-1],nodes[1:]):
		for edge in n1.iedges():
			if edge.a in n2.points:
				portal_edges.append(edge)
				break
		else:
			raise Exception("Nodes are not adjacent")

	return portal_edges


def funnel(p_depart, p_arrive, portal_edges):
	smooth_path = [p_depart]
	segDroit = Segment(p_depart, portal_edges[0][0])
	segGauche = Segment(p_depart, portal_edges[0][1])
	for edge in portal_edges[1:]:
		droit,gauche = edge
		if segGauche.pos_point(droit) > 0: # croise
			smooth_path.append(segGauche.b)
			segDroit = Segment(smooth_path[-1], droit)
			segGauche = Segment(smooth_path[-1], gauche)
			continue
		if segDroit.pos_point(gauche) < 0: # croise
			smooth_path.append(segDroit.b)
			segDroit = Segment(smooth_path[-1], droit)
			segGauche = Segment(smooth_path[-1], gauche)
			continue
		if segGauche.pos_point(gauche) > 0:
			segGauche.b = gauche
		if segDroit.pos_point(droit) < 0:
			sefDroit.b = droit
	
	smooth_path.append(p_arrive)

	return smooth_path
			

import doctest
doctest.testfile("doctest/funnel.txt")

