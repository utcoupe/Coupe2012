
from math import *

from cgal.partition import *
from subprocess import *
from area import *
from pathfinding import *
from funnel import *
from load import *
from vertexnode import *




class NavGraph:
	def __init__(self, offset=1):
		self.areas = {}
		self.vertices = {}
		self.width = 0
		self.height = 0
		self.container = None
		self.obstacles = []
		self.dynamic_obstacles = []
		self.offset = offset
		self.partition = Partition()
		self.pathfinding_area = Pathfinding(self.areas.values(), NAVGRAPH)
		self.pathfinding_vertices = Pathfinding(self.vertices.values(), NAVGRAPH)

	def calc_areas(self):
		for k in tuple(self.areas.keys()):
			self.areas.pop(k)
		self.container = Poly(((0,0),(self.width,0),(self.width,self.height),(0,self.height)))
		request = self.partition.make_request(self.container.points, self.obstacles + list(map(lambda p: p.points, self.dynamic_obstacles)), self.offset)
		self.partition.calc(request)
		for i,poly in self.partition.polygons.items():
			self.areas[i] = Area(poly.points)
			self.areas[i].color = 'black'
			self.areas[i].outline = 'white'
		for i,poly in self.partition.polygons.items():
			neighbors = [ self.areas[neighbor] for neighbor in poly.neighbors ]
			self.areas[i].init(i, self.areas[i].middle, neighbors)

	def calc_vertex_graph(self):
		for k in tuple(self.vertices.keys()):
			self.vertices.pop(k)

		# création des vertices
		for area in self.areas.values():
			for p in area.points:
				if p not in self.vertices:
					self.vertices[p] = VertexNode(p)

		# ajout des liaisons et des areas
		for area in self.areas.values():
			for p in area.points:
				self.vertices[p].areas.append(area)
				for pp in area.points:
					if pp != p:
						self.vertices[p].neighbors.append(self.vertices[pp])

		# tri des areas dans le sens trigo
		for vertex in self.vertices.values():
			vertex.areas.sort(key=lambda a: (a.pos - vertex.pos).angle())

	def load_xml(self, filename):
		"""
			Charger une carte version xml
		"""
		self.width, self.height, polys = load_xml(filename)
		self.obstacles = list([ poly.points for poly in polys])
		self.calc_areas()
		self.calc_vertex_graph()

	def get_path(self, p_depart, p_arrive):
		return self.get_path_vertex_mode(p_depart, p_arrive)
		#return self.get_path_areas_mode(p_depart, p_arrive)

	def get_path_vertex_mode(self, p_depart, p_arrive):
		p_depart = Vec(p_depart)
		p_arrive = Vec(p_arrive)
		area_depart = self.find_area_for_point(p_depart)
		area_arrive = self.find_area_for_point(p_arrive)
		remove_arrive_at_end = True
		
		# le point d'arrivé est inateignable
		if not area_arrive:
			return [],[],[]
			
		if p_depart in self.vertices:
			vertex_depart = self.vertices[p.depart]
		else:
			vertex_depart = Node()
			vertex_depart.init(p_depart.__hash__(),p_depart, [])
			if area_depart:
				for p in area_depart.points:
					vertex_depart.neighbors.append(self.vertices[p])
			else:
				near_vertex = min(self.vertices.values(), key=lambda vertex: (p_depart - vertex.pos).norm2())
				vertex_depart.neighbors.append(self.vertices[near_vertex.pos])
		if p_arrive not in self.vertices:
			vertex_arrive = Node()
			vertex_arrive.init(p_arrive.__hash__(),p_arrive, [])
			for p in area_arrive.points:
				self.vertices[p].neighbors.append(vertex_arrive)
			remove_arrive_at_end = True

		# application du pathfinding
		vertices, raw_path = self.pathfinding_vertices.compute_path(vertex_depart, vertex_arrive)

		
		# remove des vertices ajoutées
		if remove_arrive_at_end:
			for p in area_arrive.points:
				self.vertices[p].neighbors.remove(vertex_arrive)

		# smooth path
		if len(vertices) > 2:
			smooth_path = raw_path
			id_areas = set()
			for vertex in vertices[1:-1]:
				for area in vertex.areas:
					id_areas.add(area.id)
			for area in self.areas.values():
				if area.id not in id_areas:
					area.walkable = False
			_areas, _raw_path, smooth_path = self.get_path_areas_mode(p_depart, p_arrive)
			for area in self.areas.values():
				area.walkable = True
		elif len(vertices) == 2:
			smooth_path = raw_path
		else:
			raw_path = [p_depart, p_arrive]
			smooth_path = [p_depart, p_arrive]
		return vertices, raw_path, smooth_path

	def get_path_areas_mode(self, p_depart, p_arrive):
		p_depart = Vec(p_depart)
		p_arrive = Vec(p_arrive)
		area_depart = self.find_area_for_point(p_depart)
		area_arrive = self.find_area_for_point(p_arrive)
		
		# le point d'arrivé est inateignable
		if not area_arrive:
			return [],[],[]
			
		# aucune aire n'a été trouvée pour le point de départ, on va prendre la plus proche
		if not area_depart:
			area_depart = min(self.areas.values(), key=lambda a: (a.middle - p_depart).norm2())

		#application du pathfinding
		areas, raw_path = self.pathfinding_area.compute_path(area_depart, area_arrive)

		# smooth path
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
		l = []
		for n in self.vertices.values():
			p = Poly().initFromCircle(n.pos, 20, 5)
			p.color = 'yellow'
			l.append(p)
		ll = []
		for v in self.vertices.values():
			for n in v.neighbors:
				p = Poly((n.pos,n.pos+(1,0),v.pos+(1,0),v.pos))
				p.outline = 'green'
				ll.append(p)
		self.container.neighbors = []
		return [self.container] + list(self.areas.values()) + l + ll

	def add_dynamic_obstacle(self, poly):
		self.dynamic_obstacles.append(poly)

	def update(self):
		""" fonction à appeller après avoir bougé un obstacle dynamic """
		self.calc_areas()
		self.calc_vertex_graph()


if __name__ == "__main__":
	import time
	import sys
	filename = "map.xml"
	offset= sys.argv[1]
	ng = NavGraph(offset)
	start = time.time()
	ng.load_xml(filename)
	print("xml load time : %s" % (time.time() - start))
	
	sys.path.append("../visualisation")
	from graphview import *

	dynamic_obstacle = Poly().initFromCircle((1500,1500),200,8)
	ng.add_dynamic_obstacle(dynamic_obstacle)
	ng.update()
	
	v = GraphView(ng,dynamic_obstacle)
	v.mainloop()
