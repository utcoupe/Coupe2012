
from math import *

class Node:
	def __init__(self):
		self.id = -1
		self.pos = (0,0)
		self.neighbors = []

	def init(self, id, pos, neighbors):
		self.id = id
		self.pos = pos
		self.neighbors = neighbors

	def reset(self):
		self.P = 0			# poids actuel
		self.H = 0			# heuristique (ici distance de manhattan)
		self.visited = False 
		self.previous_in_path = None

	def liaison(self, node):
		return sqrt((self.pos[0]-node.pos[0])**2+(self.pos[1]-node.pos[1])**2)

	def calc_heuristique(self, arrive):
		self.H = abs(self.pos[0] - arrive.pos[0]) + abs(self.pos[1] - arrive.pos[1])

	def __repr__(self):
		return "Node(#%s, pos=%s)" % (self.id, self.pos)



class Pathfinding:
	def __init__(self, nodes):
		self.node_depart = None
		self.node_arrive = None
		self.nodes = nodes
		self.path = []

	def compute_path(self, node_depart, node_arrive):
		self.node_depart = node_depart
		self.node_arrive = node_arrive
		self.reset_nodes()
		node_depart.visited = True
		l = [node_depart]
		while l:
			current_node = self.best_node(l)
			l.remove(current_node)
			if current_node == node_arrive:
				break
			for neighbor in current_node.neighbors:
				liaison = current_node.liaison(neighbor)
				if not neighbor.visited:
					neighbor.calc_heuristique(node_arrive)
					neighbor.P = current_node.P + liaison
					neighbor.previous_in_path = current_node
					neighbor.visited = True
					l.append(neighbor)
				elif neighbor.P > current_node.P + liaison:
					neighbor.previous_in_path = current_node
					neighbor.P = current_node.P + liaison
					if neighbor not in l:
						l.append(neighbor)
		
		self.path = []

		if current_node == node_arrive:
			while current_node:
				self.path.append(current_node)
				current_node = current_node.previous_in_path
			self.path.reverse()

		return self.path

	def reset_nodes(self):
		for n in self.nodes:
			n.reset()

	def best_node(self, l):
		return min(l, key=lambda n: n.H + n.P)

		

import doctest
doctest.testfile("doctest/pathfinding.txt")

