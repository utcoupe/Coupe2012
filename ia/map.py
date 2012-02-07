
from iavec import *
from node import *
from pathfinding import *
import tkinter as tk
import xml.dom.minidom






class Map(tk.Tk):
	def __init__(self, filename):
		tk.Tk.__init__(self,None)
		self._initialize()
		self.node_depart = None
		self.node_arrive = None
		self.raw_path = []
		self.smooth_path = []
		self.circles = []
		self.recs = []
		self.nodes = [ [ Node(i,j,i*NODE_SIZE+NODE_SIZE/2,j*NODE_SIZE+NODE_SIZE/2,NODE_SIZE) for j in range(int(HEIGHT/NODE_SIZE)) ] for i in range(int(WIDTH/NODE_SIZE)) ]
		self.collision_matrix = [[ False for __ in range(HEIGHT)] for _ in range(WIDTH)]
		self.pathfinder = Pathfinder(self.nodes, self.collision_matrix)
		self.load_file(filename)
		
	def _initialize(self):
		self.grid()
		self._initialize_canvas()
		self._initialize_buttons()
		self.canvas.grid(row=0, column=0)
		self.buttonsFrame.grid(row=0, column=1)

	def _initialize_canvas(self):
		## graphique
		self.canvas = tk.Canvas(self, width=600, height=400, background='white')
		## bindings
		self.canvas.bind('<Button-3>',self.onmouseleft)
		self.canvas.bind('<B3-Motion>',self.onmouseleft)
		self.canvas.bind('<Button-1>',self.onmouseright)
		self.canvas.bind('<B1-Motion>',self.onmouseright)
		## variables
		self.id_raw_path = None
		self.id_smooth_path = None
		self.id_depart = None
		self.id_arrive = None
		self.ids_recs = []
		self.ids_circles = []
		self.ids_nodes = []

	def _initialize_buttons(self):
		self.buttonsFrame = tk.Frame(self, relief= "solid", bd= 2)
		self.buttonsFrame.grid()
		## show/hide raw path
		self.button_active_show_malus = tk.Button(self.buttonsFrame,text="Show/Hide raw path",command = self.onButtonSwitchRawPath)
		self.button_active_show_malus.grid(row=0, column=0)
		## show/hide smooth path
		self.button_active_show_malus = tk.Button(self.buttonsFrame,text="Show/Hide smooth path",command = self.onButtonSwitchSmoothPath)
		self.button_active_show_malus.grid(row=0, column=1)
		## show/hide malus
		self.button_active_show_malus = tk.Button(self.buttonsFrame,text="Show/Hide malus",command = self.onButtonSwitchMalus)
		self.button_active_show_malus.grid(row=2, column=0)
		## variables
		self.show_malus = False
		self.show_raw_path = False
		self.show_smooth_path = True
		
	def delete_pathfinding(self):
		self.delete_depart(self)
		self.delete_arrive(self)
		self.delete_raw_path(self)
		self.delete_smooth_path(self)

	def delete_depart(self):
		self.canvas.delete(self.id_depart)
		
	def delete_arrive(self):
		self.canvas.delete(self.id_arrive)

	def delete_raw_path(self):
		self.canvas.delete(self.id_raw_path)

	def delete_smooth_path(self):
		self.canvas.delete(self.id_smooth_path)

	def draw_pathfinding(self):
		self.draw_depart()
		self.draw_arrive()
		self.draw_raw_path()
		self.draw_smooth_path()

	def draw_depart(self):
		self.delete_depart()
		if self.node_depart:
			self.id_depart = self.canvas.create_rectangle(self.node_depart.to_rec(), fill='blue')
		
	def draw_arrive(self):
		self.delete_arrive()
		if self.node_arrive:
			self.id_arrive = self.canvas.create_rectangle(self.node_arrive.to_rec(), fill='blue')

	def draw_raw_path(self):
		self.delete_raw_path()
		if self.raw_path and self.show_raw_path:
			self.id_raw_path = self.canvas.create_line(self.raw_path, fill='red')

	def draw_smooth_path(self):
		self.delete_smooth_path()
		if self.smooth_path and self.show_smooth_path:
			self.id_smooth_path = self.canvas.create_line(self.smooth_path, fill='green')

	
	def load_file(self,filename):
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
		self.canvas.delete(tk.ALL)
		self.canvas_recs = []
		self.canvas_circles = []
		self.canvas_nodes = []
		self.draw_circles()
		self.draw_recs()
		self.draw_nodes()
		self.draw_pathfinding()

	def delete_nodes(self):
		for o in self.canvas_nodes:
			self.canvas.delete(o)
		self.canvas_nodes = []
	
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
		self.canvas_circles.append(self.canvas.create_oval(x-r,y-r,x+r,y+r))

	def draw_rec(self,pos):
		self.canvas_recs.append(self.canvas.create_rectangle(pos))

	def draw_node(self,node):
		if not node.collide:
			self.canvas_nodes.append(self.canvas.create_rectangle(node.to_rec(), fill=self.node_to_color(node)))

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
	
	def update_nodes(self):
		for i in range(len(self.nodes)):
			for j in range(len(self.nodes[0])):
				self.nodes[i][j].calc_collide(self.collision_matrix)

	def onmouseleft(self,event):
		x,y = event.x,event.y
		r = self.get_node_from_pos(x,y)
		if r:
			self.depart = IAVec((r[0],r[1]))
			self.node_depart = self.nodes[self.depart[0]][self.depart[1]]
			self.draw_depart()
			if self.node_arrive:
				self.show_path()
		else:
			print("can't find node")

	def onmouseright(self,event):
		x,y = event.x,event.y
		r = self.get_node_from_pos(x,y)
		if r:
			self.node_arrive = self.nodes[r[0]][r[1]]
			self.draw_arrive()
			if self.node_depart:
				self.show_path()
		else:
			print("can't find node")
	
	def show_path(self):
		self.pathfinder.compute_path(self.node_depart, self.node_arrive)
		if self.pathfinder.raw_path_ok():
			self.raw_path = self.pathfinder.raw_path_to_canvas_lines_pos()
		else:
			self.raw_path = []
			print("no path found")
		if self.pathfinder.smooth_path_ok():
			self.smooth_path = self.pathfinder.smooth_path_to_canvas_lines_pos()
		else:
			self.smooth_path = []
			print("no smooth path found")
		if self.show_malus:
			self.draw()
		else:
			self.draw_raw_path()
			self.draw_smooth_path()
			
	
	def get_node_from_pos(self,x,y):
		for i in range(len(self.nodes)):
			for j in range(len(self.nodes[0])):
				node = self.nodes[i][j]
				x1,y1,x2,y2 = node.to_rec()
				if x1 <= x <= x2 and y1 <= y <= y2:
					return (i,j,node)
		return None

	def node_to_color(self,node):
		if self.show_malus:
			if node.visited:
				intensite = min(255, max(0, 255-int(node.malus * 255 / (WIDTH+HEIGHT))))
				s_intensite = hex(intensite)[2:]
				if len(s_intensite) == 1:
					s_intensite = '0' + s_intensite
				return '#'+ s_intensite + s_intensite + '00' 
			else:
				return 'white'
		else:
			return 'white'
			

	def onButtonSwitchMalus(self):
		self.show_malus = not self.show_malus
		self.draw()

	def onButtonSwitchRawPath(self):
		self.show_raw_path = not self.show_raw_path
		self.draw_pathfinding()

	def onButtonSwitchSmoothPath(self):
		self.show_smooth_path = not self.show_smooth_path
		self.draw_pathfinding()

		

import doctest
doctest.testfile("doctest/map.txt")
