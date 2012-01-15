
from vec import *
from node import *
from pathfinding import *
import tkinter as tk
import xml.dom.minidom






class Map:
	def __init__(self, filename, canvas):
		self.collision_matrix = [[ False for __ in range(HEIGHT)] for _ in range(WIDTH)]
		self.canvas = canvas
		self.pathfinder_lines = None
		self.pathfinder_smooth_lines = None
		self.canvas_depart = None
		self.canvas_arrive = None
		self.canvas.bind('<Button-1>',self.onmouseleftdown)
		self.canvas.bind('<Button-3>',self.onmouserightdown)
		self.depart = None
		self.circles = []
		self.recs = []
		self.init_nodes()
		with open(filename) as f:
			document = f.read()
			f.close()
	
			dom = xml.dom.minidom.parseString(document)
			for circle in dom.getElementsByTagName("circle"):
				self.add_circle(int(circle.getAttribute("x")),int(circle.getAttribute("y")),int(circle.getAttribute("r")))
				
			for rec in dom.getElementsByTagName("rectangle"):
				self.add_rec(
					int(rec.getAttribute("x1")),int(rec.getAttribute("y1")),
					int(rec.getAttribute("x2")),int(rec.getAttribute("y2")))
		self.update_nodes()

	
	def draw(self):
		self.draw_circles()
		self.draw_recs()
		self.draw_nodes()

	def print_collision_matrix(self):
		for i in range(WIDTH):
			for j in range(HEIGHT):
				print( '1' if self.collision_matrix[i][j] else '0', end='')
			print('')

	def add_circle(self,x,y,r):
		self.circles.append((x,y,r))
		r2 = r*r
		for i in range(-r,r+1,1):
			i2 = i*i
			for j in range(-r,r+1,1):
				if i2+j*j <= r2:
					self.collision_matrix[x+i][y+j] = True

	def add_rec(self,x1,y1,x2,y2):
		self.recs.append((x1,y1,x2,y2))
		for i in range(x1,x2+1,1):
			for j in range(y1,y2+1,1):
				self.collision_matrix[i][j] = True
	
	def draw_circle(self,pos):
		x,y,r = pos
		self.canvas.create_oval(x-r,y-r,x+r,y+r)

	def draw_rec(self,pos,**params):
		self.canvas.create_rectangle(pos,**params)

	def draw_node(self,node):
		if not node.collide:
			self.draw_rec(node.to_rec())

	def draw_circles(self):
		for c in self.circles:
			self.draw_circle(c)

	def draw_recs(self):
		for r in self.recs:
			self.draw_rec(r)

	def draw_nodes(self):
		for i in range(len(self.nodes)):
			for j in range(len(self.nodes[0])):
				self.draw_node(self.nodes[i][j])
	
	def init_nodes(self,x1=0,y1=0,x2=WIDTH,y2=HEIGHT):
		self.nodes = [ [ Node(i,j,i*NODE_SIZE+NODE_SIZE/2,j*NODE_SIZE+NODE_SIZE/2,NODE_SIZE) for j in range(int(HEIGHT/NODE_SIZE)) ] for i in range(int(WIDTH/NODE_SIZE)) ]

	def update_nodes(self):
		for i in range(len(self.nodes)):
			for j in range(len(self.nodes[0])):
				self.nodes[i][j].calc_collide(self.collision_matrix)

	def onmouseleftdown(self,event):
		x,y = event.x,event.y
		r = self.get_node_from_pos(x,y)
		if r:
			self.depart = Vec((r[0],r[1]))
			node_depart = self.nodes[self.depart[0]][self.depart[1]]
			if self.canvas_depart:
				self.canvas.delete(self.canvas_depart)
			if self.canvas_arrive:
				self.canvas.delete(self.canvas_arrive)
			if self.pathfinder_lines:
				self.canvas.delete(self.pathfinder_lines)
			if self.pathfinder_smooth_lines:
				self.canvas.delete(self.pathfinder_smooth_lines)
			self.canvas_depart = self.canvas.create_rectangle(node_depart.to_rec(), fill='green')
		else:
			print("can't find node")

	def onmouserightdown(self,event):
		x,y = event.x,event.y
		r = self.get_node_from_pos(x,y)
		if self.depart and r:
			if self.canvas_arrive:
				self.canvas.delete(self.canvas_arrive)
			if self.pathfinder_lines:
				self.canvas.delete(self.pathfinder_lines)
			if self.pathfinder_smooth_lines:
				self.canvas.delete(self.pathfinder_smooth_lines)
			node_depart = self.nodes[self.depart[0]][self.depart[1]]
			node_arrive = self.nodes[r[0]][r[1]]
			self.canvas_arrive = self.canvas.create_rectangle(node_arrive.to_rec(), fill='red')
			self.pathfinder(node_depart, node_arrive)
		else:
			print("can't find node")

	def pathfinder(self, node_depart, node_arrive):
		p = Pathfinder(self.nodes, self.collision_matrix)
		p.compute_path(node_depart, node_arrive)
		if p.raw_path_ok():
			self.pathfinder_lines = self.canvas.create_line(p.raw_path_to_canvas_lines_pos(), fill='red')
		else:
			print("no path found")
		if p.smooth_path_ok():
			self.pathfinder_smooth_lines = self.canvas.create_line(p.smooth_path_to_canvas_lines_pos(), fill='green')
		else:
			print("no smooth path found")
	
	def get_node_from_pos(self,x,y):
		for i in range(len(self.nodes)):
			for j in range(len(self.nodes[0])):
				node = self.nodes[i][j]
				x1,y1,x2,y2 = node.to_rec()
				if x1 <= x <= x2 and y1 <= y <= y2:
					return (i,j,node)
		return None



import doctest
doctest.testfile("doctest/map.txt")
