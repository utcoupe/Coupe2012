
from math import *


TILED		= 0
NAVGRAPH	= 1

class AStar:
	def __init__(self, nodes, t):
		self.node_depart = None
		self.node_arrive = None
		self.nodes = nodes
		self.node_path = []
		self.raw_path = []
		self.t = t

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
				if neighbor.walkable:
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
		
		self.node_path = []

		if current_node == node_arrive:
			while current_node:
				self.node_path.append(current_node)
				current_node = current_node.previous_in_path
			self.node_path.reverse()

		self.raw_path = list([ node.pos for node in self.node_path ])

		return self.node_path, self.raw_path

	def reset_nodes(self):
		if self.t == TILED:
			for line in self.nodes:
				for n in line:
					n.reset()
		else:
			for n in self.nodes:
				n.reset()

	def best_node(self, l):
		return min(l, key=lambda n: n.H + n.P)

		


