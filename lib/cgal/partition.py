

import sys
import os

import subprocess
import time


class PartitionPoly:
	def __init__(self, id, points, neighbors):
		self.id = id
		self.points = points
		self.neighbors = neighbors

	def __repr__(self):
		return "PartitionPoly(%s,%s,%s)" % (self.id, self.points, self.neighbors)
		
"""
class Partition:
	def __init__(self):
		self.polygons = {}
		self.cmd = os.path.join(os.path.dirname(os.path.abspath(__file__)), "partition")

	def make_request(self, container, obstacles, offset=1):
		s = str(offset)+"\n"
		s+= self._poly_to_str(container)
		s+=self._make_obstacles_request(obstacles)
		return s

	def _make_obstacles_request(self, obstacles):
		s = "%s\n" % len(obstacles)
		for o in obstacles:
			s+= self._poly_to_str(o)
		return s

	def calc(self, request):
		self.polygons = {}
		p = Popen(self.cmd, stdout=PIPE, stdin=PIPE)
		result = str(p.communicate(bytes(request,"utf-8"))[0], "utf-8")
		self.analyse_result(result)

	def analyse_result(self, result):
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
"""
class Partition:
	def __init__(self, offset, container, obstacles, init_request=None):
		self.polygons = {}
		self.cmd = os.path.join(os.path.dirname(os.path.abspath(__file__)), "partition_continue")
		self.p = subprocess.Popen(self.cmd, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
		if init_request:
			self.p.stdin.write(init_request.encode("utf-8"))
		else:
			self.p.stdin.write(self.init_to_str(offset, container, obstacles).encode("utf-8"))
		self.p.stdin.flush()

	def init_to_str(self, offset, container, obstacles):
		s  = self.offset_to_str(offset)
		s += self.poly_to_str(container)
		s += self.obstacles_to_str(obstacles)
		return s
	
	def offset_to_str(self, offset):
		return str(offset)+'\n'
	
	def obstacles_to_str(self, obstacles):
		s = "%s\n" % len(obstacles)
		for o in obstacles:
			s+= self.poly_to_str(o)
		return s
	
	def poly_to_str(self, poly):
		s = "%s\n" % len(poly)
		for p in poly:
			s += "%s %s\n" % (p[0],p[1])
		return s
		
	def calc(self, request):
		self.polygons = {}
		self.p.stdin.write(request.encode("utf-8"))
		self.p.stdin.flush()
		self.p.stdout.flush()
		str_n_polys = self.p.stdout.readline().decode("utf-8")
		n_polys = int(str_n_polys)
		#print("N_POLYS", n_polys)
		result  = ""
		for i in range(n_polys):
			result += self.p.stdout.readline().decode("utf-8")
			#print(result)
		self.analyse_result(result)
		
	def analyse_result(self, result):
		self.polygons = {}
		for str_poly in result.strip().split('\n'):
			d = {}
			str_to_exec = str_poly
			exec(str_to_exec, d)
			poly = PartitionPoly(d['id'], d['points'], d['neighbors'])
			self.polygons[poly.id] = poly
				
		for poly in self.polygons.values():
			to_remove = []
			for n in poly.neighbors:
				if n not in self.polygons:
					to_remove.append(n)
			for n in to_remove:
				poly.neighbors.remove(n)

		#for poly in self.polygons.values():
		#	print(poly)

	def stop(self):
		self.p.terminate()
		time.sleep(1)
		if not self.p.poll():
			self.p.kill()
		
