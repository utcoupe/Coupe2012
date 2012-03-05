#coding -*- utf-8 -*-


import tkinter as tk
import re
from partition import *

WIDTH		= 600
HEIGHT		= 400

class Poly:
	def __init__(self):
		self.points = []
		self.id = -1
		self.neighbors = []
		self._middle = None

	def middle(self):
		if not self._middle:
			sum_x = 0
			sum_y = 0
			for p in self.points:
				sum_x += p[0]
				sum_y += p[1]
			n = len(self.points)
			self._middle = (sum_x/n, sum_y/n)
		return self._middle

	

class Window(tk.Tk):
	def __init__(self, init_file):
		tk.Tk.__init__(self,None)
		self.canvas = tk.Canvas(self, width=WIDTH, height=HEIGHT, background='white')
		self.canvas.pack()
		self.canvas.bind("<Button-1>", self.callback)
		self.polygons = {}
		f = open(filename)
		self.partition = Partition(None,None,None,f.read())
		f.close()

	def callback(self, event):
		print(event.x / self.w_to_px, event.y / self.h_to_px)
				
	def draw_polygons(self):
		self.canvas.delete(tk.ALL)
		maxi_x = -1E20
		maxi_y = -1E20
		mini_x = 1E20
		mini_y = 1E20
		for poly in self.polygons.values():
			for point in poly.points:
				maxi_x = max(maxi_x,point[0])
				maxi_y = max(maxi_y,point[1])
				mini_x = min(mini_x,point[0])
				mini_y = min(mini_y,point[1])
		
		w_to_px = WIDTH / float(maxi_x - mini_x)
		h_to_px = HEIGHT / float(maxi_y - mini_y)
		print (maxi_x, maxi_y, mini_x, mini_y, w_to_px, h_to_px)
		self.w_to_px = w_to_px
		self.h_to_px = h_to_px
		
		
		for poly in self.polygons.values():
			path = []
			for point in poly.points:
				path.append(point[0] * w_to_px)
				path.append(point[1] * h_to_px)
			self.canvas.create_polygon(path, outline='green')
		
		"""for poly in self.polygons.values():
			for n in poly.neighbors:
				if n in self.polygons:
					neighbor = self.polygons[n]
					points = [poly.middle()[0] * w_to_px, poly.middle()[1] * h_to_px, neighbor.middle()[0] * w_to_px, neighbor.middle()[1] * h_to_px]
					self.canvas.create_line(points, fill='blue')
		"""
		
	def load_file(self,filename):
		f = open(filename)
		request = f.read()
		f.close()
		self.partition.calc(request)
		self.polygons = self.partition.polygons


import sys
import threading

def loop():
	"""while 1:
		filename = input("filename?")
		w.load_file(filename)
		w.draw_polygons()"""
	filename = "data/obstacles/data.cin"
	w.load_file(filename)
	w.draw_polygons()
	input()
	filename = "data/obstacles/data2.cin"
	w.load_file(filename)
	w.draw_polygons()
	input()
	filename = "data/obstacles/data3.cin"
	w.load_file(filename)
	w.draw_polygons()
	w.partition.stop()
		
filename = sys.argv[1]
w = Window(filename)
t = threading.Thread(target=loop)
t.setDaemon(True)
t.start()
w.mainloop()
