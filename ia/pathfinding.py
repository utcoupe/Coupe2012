
import time
from copy import copy

from node import *
from nodelist import *

class Pathfinder:
	def __init__(self, nodes, collision_matrix):
		self.nodes = nodes
		self.raw_path = []
		self.smooth_path = []
		self.collision_matrix = collision_matrix

	def raw_path_ok(self):
		return len(self.raw_path) > 1

	def smooth_path_ok(self):
		return len(self.smooth_path) > 1
	
	def compute_path(self, node_depart, node_arrive):
		self.compute_raw_path(node_depart, node_arrive)
		self.compute_smooth_path(self.raw_path)
		return self
	
	def compute_raw_path(self, node_depart, node_arrive):
		time_start = time.time()
		# init
		for i in range(len(self.nodes)):
			for j in range(len(self.nodes[0])):
				self.nodes[i][j].reset()
		node_depart.visited = True
		l = NodeList(node_depart)
		print ("pathfinding %s -> %s init done" % (node_depart.pos,node_arrive.pos))
		while not l.is_empty():
			#print (l)
			current = l.pop_min_score()
			#print (current)
			if current.pos == node_arrive.pos:
				break
			for d in range(8):
				ni,nj = next_pos(d,current.pos)
				if 0 <= ni < len(self.nodes) and 0 <= nj < len(self.nodes[0]):
					next_node = self.nodes[ni][nj]
					if not next_node.collide:
						if not next_node.visited:
							next_node.set_cost_to_arrive(node_arrive)
						if next_node.try_set_previous(current):
							l.push_uniq(next_node)

		self.raw_path = node_arrive.get_path()
		print ("calc_raw_path : %s ms" % ((time.time() - time_start)*1000.0))
		return self

	def compute_smooth_path(self, path):
		time_start = time.time()
		if len(path) > 2:
			new_path = copy(path)
			check_point = path[0]
			current = path[1]
			check_point_dir = check_point.pos.dir_to(current.pos)
			for node in path[2:]:
				if self.walkable(check_point,node,check_point_dir):
					new_path.remove(current)
				else:
					check_point = current
					check_point_dir = check_point.pos.dir_to(node.pos)
				current = node
			self.smooth_path = new_path
		print ("calc_smooth_path : %s ms" % ((time.time() - time_start)*1000.0))

	def walkable(self, node1, node2, current_dir):
		if current_dir:
			d = node1.pos.dir_to(node2.pos)
			if d == current_dir:
				return True
		m = node2.real_pos - node1.real_pos
		length = round(m.norm())+1
		m.normalize()
		point = Vec(node1.real_pos)
		for t in range(1,length,4):
			new_point = point + (m * t)
			new_point.round()
			x,y = new_point
			if self.collision_matrix[x][y]:
				return False
		return True
		
	

	def raw_path_to_canvas_lines_pos(self):
		return self.path_to_canvas_lines_pos(self.raw_path)

	def smooth_path_to_canvas_lines_pos(self):
		return self.path_to_canvas_lines_pos(self.smooth_path)

	def path_to_canvas_lines_pos(self, path):
		canvas_path = []
		for n in path:
			x,y = n.real_pos
			canvas_path.append(x)
			canvas_path.append(y)
		return canvas_path
		

























