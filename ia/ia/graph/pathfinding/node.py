
from math import *

class Node:
	def __init__(self):
		self.id = -1
		self.pos = (0,0)
		self.neighbors = []
		self.walkable = True
		self.reset()

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
