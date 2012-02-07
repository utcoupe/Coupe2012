
	

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
		else:
			raise Exception("%s n'est dans aucune zone" % (p,))
	
if __name__ == "__main__":
	import time
	import sys
	filename = sys.argv[1]
	ng = NavGraph()
	ng.load_xml(filename)
	
	sys.path.append("../view")
	from interactiveview import *
	
	p_depart = (200,200)
	p_arrive = (1500,1500)

	def onLeft(x,y):
		global p_arrive
		p_arrive = (x,y)
		calc_path()

	def onRight(x,y):
		global p_depart
		p_depart = (x,y)
		calc_path()

	def calc_path():
		start = time.time()
		areas,raw_path,smooth_path = ng.get_path(p_depart,p_arrive)
		print(time.time() - start)
		v.clear()
		v.draw_polygons(ng.areas.values())
		v.draw_line(raw_path, 'red')
		v.draw_line(smooth_path, 'blue')
	
	v = InteractiveView(onLeft,onRight,onLeft,onRight)
	v.draw_polygons(ng.areas.values())
	v.mainloop()
