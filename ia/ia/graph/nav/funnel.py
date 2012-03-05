

from geometry import Segment


def polys_to_portal_edges(nodes):
	portal_edges = []
	for n1,n2 in zip(nodes[:-1],nodes[1:]):
		for edge in n1.iedges():
			if edge.a in n2.points and edge.b in n2.points:
				portal_edges.append(edge)
				break
		else:
			raise Exception("Nodes are not adjacent")
	
	return portal_edges


def funnel(p_depart, p_arrive, portal_edges):
	smooth_path = [p_depart]
	segDroit = Segment(p_depart, portal_edges[0][0])
	segGauche = Segment(p_depart, portal_edges[0][1])
	i_gauche = i_droit = i = 0
	portal_edges.append((p_arrive,p_arrive))
	while smooth_path[-1] != p_arrive:
		i += 1
		if i >= len(portal_edges):
			smooth_path.append(p_arrive)
			break
		#print (smooth_path)
		#print(i, i_droit, i_gauche)
		#print(segDroit.b, segGauche.b)
		droit = portal_edges[i][0]
		gauche = portal_edges[i][1]
		#print(droit, gauche)
		if segGauche.pos_point(droit) > 0: # croise
			#print("cgauche")
			smooth_path.append(segGauche.b)
			i_gauche += 1
			i = i_droit = i_gauche
			segDroit = Segment(smooth_path[-1], portal_edges[i][0])
			segGauche = Segment(smooth_path[-1], portal_edges[i][1])
			continue
		if segDroit.pos_point(gauche) < 0: # croise
			#print("cdroit")
			smooth_path.append(segDroit.b)
			i_droit += 1
			i = i_gauche = i_droit
			segDroit = Segment(smooth_path[-1], portal_edges[i][0])
			segGauche = Segment(smooth_path[-1], portal_edges[i][1])
			continue
		if segGauche.pos_point(gauche) <= 0:
			#print("thingauche")
			segGauche.b = gauche
			i_gauche = i
			if gauche == p_arrive:
				smooth_path.append(p_arrive)
				break
		if segDroit.pos_point(droit) >= 0:
			#print("thindroit")
			segDroit.b = droit
			i_droit = i
			if droit == p_arrive:
				smooth_path.append(p_arrive)
				break
	

	return smooth_path
			


