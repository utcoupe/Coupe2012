

from vec import *
from params import *


class Node:
	def __init__(self,i,j,x,y,size):
		self.pos = Vec((int(i),int(j)))
		self.real_pos = Vec((int(x),int(y)))
		self.size = size
		self.collide = False
		self.current_cost = 0
		self.malus = 0
		self.score = 0
		self.visited = False		
		self.previous_node = None		# previous node to get here
		self.in_open_list = False		# is the node in the list of pathfinder
		self.previous_in_open = None
		self.next_in_open = None

	def reset(self):
		self.visited = False
		self.in_open_list = False
		self.previous_node = None
		self.previous_in_open = None
		self.next_in_open = None
		
	def to_rec(self):
		vec_half_size = (int(self.size/2),int(self.size/2))
		return list(self.real_pos-vec_half_size) + list(self.real_pos+vec_half_size)

	def calc_collide(self,collision_matrix):
		x1,y1,x2,y2 = self.to_rec()
		for i in range(x1,x2,1):
			for j in range(y1,y2,1):
				if i >= WIDTH or j >= HEIGHT or collision_matrix[i][j]:
					self.collide = True
					return True
		self.collide = False
		return False

	def dist_manhattan(self, node):
		return sum(map(abs, self.real_pos - node.real_pos))

	def cost_to(self,node):
		cost = self.dist_manhattan(node)
		return cost

	def set_malus(self, node_arrive):
		malus = self.dist_manhattan(node_arrive)
		self.malus = malus
		return self
	
	def try_set_previous(self, previous):
		new_current_cost = previous.current_cost
		new_current_cost += previous.cost_to(self)
		if not self.visited or new_current_cost < self.current_cost:
			self.previous_node = previous
			self.current_cost = new_current_cost
			self.update_score()
			self.visited = True
			return True
		else:
			return False

	def get_score(self):
		return self.current_cost + self.malus

	def update_score(self):
		self.score = self.get_score()
	
	def __str__(self):
		return "<Node pos=%s previous=%s>" % (self.real_pos,self.previous_node)

	def get_path(self):
		"""
		>>> n1 = Node(0,0,0,0,2)
		>>> n2 = Node(0,0,0,0,2)
		>>> n3 = Node(0,0,0,0,2)
		>>> n3.previous_node = n2
		>>> n2.previous_node = n1
		>>> n3.get_path() == [n3,n2,n1]
		True
		"""
		node = self
		path = []
		while node:
			path.append(node)
			node = node.previous_node
		return path



import doctest
doctest.testfile("doctest/node.txt")
