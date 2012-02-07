
from math import *

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
		print("%s areas" % len(self.areas))


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
				self.load_xml_polygon(xml_poly)

			# carre
			for xml_carre in dom.getElementsByTagName("carre"):
				self.load_xml_carre(xml_carre)

			# cercle
			for xml_circle in dom.getElementsByTagName("circle"):
				self.load_xml_circle(xml_circle)
			
		self.calc_areas()

	def load_xml_polygon(self, xml_poly):
		poly = Poly(((int(vertex.getAttribute("x")), int(vertex.getAttribute("y"))) for vertex in xml_poly.getElementsByTagName("vertex")))
		self.obstacles.append(poly.points)

	def load_xml_carre(self, xml_carre):
		x,y,w = int(xml_carre.getAttribute("x")), int(xml_carre.getAttribute("y")), int(xml_carre.getAttribute("width"))
		poly = Poly().initFromCarre((x,y), w, True)
		self.obstacles.append(poly.points)

	def load_xml_circle(self, xml_circle):
		x,y,r,n = int(xml_circle.getAttribute("x")), int(xml_circle.getAttribute("y")), int(xml_circle.getAttribute("rayon")), int(xml_circle.getAttribute("n"))
		poly = Poly().initFromCircle((x,y), r, n)
		print(poly.points)
		self.obstacles.append(poly.points)

	def get_path(self, p_depart, p_arrive):
		area_depart = self.find_area_for_point(p_depart)
		area_arrive = self.find_area_for_point(p_arrive)
		if not area_depart or not area_arrive:
			return [],[],[]
		areas = self.pathfinding.compute_path(area_depart, area_arrive)
		if len(areas) > 1:
			raw_path = list( area.middle for area in areas )
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
