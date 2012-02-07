
	

from cgal.partition import *
from subprocess import *
from area import *
from pathfinding import *
from funnel import *

import xml.dom.minidom



class NavGraph:
	def __init__(self):
		self.areas = {}
		self.width = 0
		self.height = 0
		self.obstacles = []
		self.partition = Partition()
		self.pathfinding = Pathfinding(self.areas.values())

	def calc_areas(self):
		request = self.partition.make_request(((0,0),(self.width,0),(self.width,self.height),(0,self.height)), self.obstacles)
		self.partition.calc(request)
		for i,poly in self.partition.polygons.items():
			self.areas[i] = Area(poly.points)
		for i,poly in self.partition.polygons.items():
			neighbors = [ self.areas[neighbor] for neighbor in poly.neighbors ]
			self.areas[i].init(i, self.areas[i].middle, neighbors)
		print(self.areas)


	def load_xml(self, filename):
		"""
			Charger une carte version xml
		"""
		self.obstacles = []
		with open(filename) as f:
			document = f.read()
			f.close()
	
			dom = xml.dom.minidom.parseString(document)

			# size
			size = dom.getElementsByTagName("size")[0]
			self.width = int(size.getAttribute("width"))
			self.height = int(size.getAttribute("height"))

			#  polygon
			for xml_poly in dom.getElementsByTagName("polygon"):
				poly = [ (int(vertex.getAttribute("x")), int(vertex.getAttribute("y"))) for vertex in xml_poly.getElementsByTagName("vertex") ]
				self.obstacles.append(poly)
		self.calc_areas()

	def get_path(self, p_depart, p_arrive):
		area_depart = self.find_area_for_point(p_depart)
		area_arrive = self.find_area_for_point(p_arrive)
		areas = self.pathfinding.compute_path(area_depart, area_arrive)
		raw_path = [ area.middle for area in areas ]
		portal_edges = polys_to_portal_edges(areas)
		smooth_path = funnel(p_depart, p_arrive, portal_edges)
		return areas,raw_path,smooth_path

	def find_area_for_point(self, p):
		for area in self.areas.values():
			if p in area:
				return area
	
if __name__ == "__main__":
	import time
	import sys
	filename = sys.argv[1]
	ng = NavGraph()
	start = time.time()
	ng.load_xml(filename)
	print(time.time() - start)
	areas,raw_path,smooth_path = ng.get_path((100,200), (1500,1000))
	print(time.time() - start)
	
	sys.path.append("../view")
	from view import *
	
	v = View()
	v.draw_polygons(ng.areas.values())
	v.draw_line(raw_path, 'red')
	v.draw_line(smooth_path, 'blue')
	v.mainloop()
