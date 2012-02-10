

import sys
import os

from subprocess import *
import re

class PartitionPoly:
	def __init__(self, id, points, neighbors):
		self.id = id
		self.points = points
		self.neighbors = neighbors
		
		sum_x = 0
		sum_y = 0
		for p in self.points:
			sum_x += p[0]
			sum_y += p[1]
		n = len(self.points)
		self._middle = (sum_x/n, sum_y/n)

	def middle(self):
		return self._middle

class Partition:
	def __init__(self):
		self.polygons = {}
		self.cmd = os.path.join(os.path.dirname(os.path.abspath(__file__)), "partition")

	def make_request(self, container, obstacles, offset=1):
		s = str(offset)+"\n"
		s+= self._poly_to_str(container)
		s+= "%s\n" % len(obstacles)
		for o in obstacles:
			s+= self._poly_to_str(o)
		return s

	def calc(self, request):
		p = Popen(self.cmd, stdout=PIPE, stdin=PIPE)
		result = str(p.communicate(bytes(request,"utf-8"))[0], "utf-8")
		spec_points = "<(.+)>"
		spec_point = "(?P<x>[0-9.]+)\s+(?P<y>[0-9.]+)"
		spec_id = "0x(?P<id>[0-9a-z]+)"
		for line in result.split("\n"):
			t = re.search(spec_points, line)
			if t:
				s = t.group(0)
				points = [ (round(float(t.group("x"))), round(float(t.group("y")))) for t in re.finditer(spec_point, s) ]
				ids = [t.group("id") for t in re.finditer(spec_id, line)]
				if ids:
					self.polygons[ids[0]] = PartitionPoly(ids[0], points, ids[1:])
		for poly in self.polygons.values():
			to_remove = []
			for n in poly.neighbors:
				if n not in self.polygons:
					to_remove.append(n)
			for n in to_remove:
				poly.neighbors.remove(n)
			

	def _poly_to_str(self, poly):
		s = "%s\n" % len(poly)
		for p in poly:
			s += "%s %s\n" % (p[0],p[1])
		return s
		
