# -*- coding: utf-8 -*-



import tkinter as tk

WIDTH	= 600
HEIGHT	= 400


class View(tk.Tk):
	def __init__(self):
		tk.Tk.__init__(self,None)
		self.canvas = tk.Canvas(self, width=WIDTH, height=HEIGHT, background='white')
		self.canvas.pack()
		self.w_to_px = 1
		self.h_to_px = 1


	def remove(self, i=tk.ALL):
		self.canvas.delete(i)
	
	def draw_polygons(self, polygons):
		maxi_x = -1E20
		maxi_y = -1E20
		mini_x = 1E20
		mini_y = 1E20
		for poly in polygons:
			for point in poly.points:
				maxi_x = max(maxi_x,point[0])
				maxi_y = max(maxi_y,point[1])
				mini_x = min(mini_x,point[0])
				mini_y = min(mini_y,point[1])
		
		self.w_to_px = WIDTH / float(maxi_x - mini_x)
		self.h_to_px = HEIGHT / float(maxi_y - mini_y)
		#print (maxi_x, maxi_y, mini_x, mini_y, self.w_to_px, self.h_to_px)

		r = []
		for poly in polygons:
			path = []
			for point in poly.points:
				path += self.point_to_px(point)
			r.append(self.canvas.create_polygon(path, outline=poly.outline, fill=poly.color))
		return r

	def draw_lines(self, lines, color='red'):
		return ( self.draw_line(line) for line in lines )
			
	def draw_line(self, line, color='red'):
		path = []
		for p in line:
			path += self.point_to_px(p)
		return self.canvas.create_line(path, fill=color)

	def point_to_px(self, p):
		return (p[0] * self.w_to_px, HEIGHT - (p[1] * self.h_to_px))
	



