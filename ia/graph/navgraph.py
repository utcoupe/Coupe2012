
from math import *

from cgal.partition import *
from subprocess import *
from area import *
from pathfinding import *
from funnel import *
from load import *




class NavGraph:
	def __init__(self):
		self.areas = {}
		self.width = 0
		self.height = 0
		self.obstacles = []
		self.partition = Partition()
		self.pathfinding = Pathfinding(self.areas.values(), NAVGRAPH)

	def calc_areas(self):
		request = self.partition.make_request(((0,0),(self.width,0),(self.width,self.height),(0,self.height)), self.obstacles)
		self.partition.calc(request)
		for i,poly in self.partition.polygons.items():
			self.areas[i] = Area(poly.points)
			self.areas[i].color = 'black'
			self.areas[i].outline = 'green'
		for i,poly in self.partition.polygons.items():
			neighbors = [ self.areas[neighbor] for neighbor in poly.neighbors ]
			self.areas[i].init(i, self.areas[i].middle, neighbors)
		print("%s areas" % len(self.areas))


	def load_xml(self, filename):
		"""
			Charger une carte version xml
		"""
		self.width, self.height, polys = load_xml(filename)
		self.obstacles = list([ poly.points for poly in polys])
		self.calc_areas()

	def get_path(self, p_depart, p_arrive):
		area_depart = self.find_area_for_point(p_depart)
		area_arrive = self.find_area_for_point(p_arrive)
		if not area_depart or not area_arrive:
			return [],[],[]
		areas, raw_path = self.pathfinding.compute_path(area_depart, area_arrive)
		if len(areas) > 1:
			portal_edges = polys_to_portal_edges(areas)
			smooth_path = funnel(p_depart, p_arrive, portal_edges)
		else:
			raw_path = [p_depart, p_arrive]
			smooth_path = [p_depart, p_arrive]
		return areas,raw_path,smooth_path

	def find_area_for_point(self, p):
		for area in self.areas.values():
			if p in area:
				return area

	def get_polygons(self):
		return self.areas.values()
	
if __name__ == "__main__":
	import time
	import sys
	filename = sys.argv[1]
	ng = NavGraph()
	start = time.time()
	ng.load_xml(filename)
	print("xml load time : %s" % (time.time() - start))
	
	sys.path.append("../view")
	from graphview import *
	
	v = GraphView(ng)
	v.mainloop()
