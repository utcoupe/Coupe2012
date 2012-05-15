
import tkinter as tk
import math

MAX_VAL  = 4000
sizeFact = 10.0
Center = (MAX_VAL,MAX_VAL) # x,y


class Radar(tk.Tk):
	def __init__(self):
		super().__init__()
		self.canvas = tk.Canvas(self, width=(2*MAX_VAL)/sizeFact, height=(MAX_VAL)/sizeFact, bg='blue')
		canv = self.canvas
		canv.create_line((Center[0]/sizeFact,Center[1]/sizeFact,Center[0]/sizeFact,(Center[1]-3000)/sizeFact),fill="red",width=2)
		canv.create_line((Center[0]/sizeFact,(Center[1]-3000)/sizeFact,(Center[0]-2000)/sizeFact,(Center[1]-3000)/sizeFact),fill="red",width=2)
		canv.create_line((Center[0]/sizeFact,(Center[1]-3000)/sizeFact,(Center[0]+2000)/sizeFact,(Center[1]-3000)/sizeFact),fill="red",width=2)
		canv.create_line(((Center[0]-2000)/sizeFact,Center[1]/sizeFact,(Center[0]-2000)/sizeFact,(Center[1]-3000)/sizeFact),fill="red",width=2)
		canv.create_line(((Center[0]+2000)/sizeFact,Center[1]/sizeFact,(Center[0]+2000)/sizeFact,(Center[1]-3000)/sizeFact),fill="red",width=2)
		self.canvas.pack()

		self.counter = 0

	def draw_points(self, points):

		rad = 5

		def f(p):
			x,y = p # y et x sont inversé à l'affichage
			x,y = MAX_VAL - y, MAX_VAL - x
			x /= sizeFact
			y /= sizeFact
			return x,y
			
		points = map(f, points)

		oldtag = 'mytag'+str(self.counter)
		self.counter += 1
		newtag = 'mytag'+str(self.counter)
		for x,y in points:
			self.canvas.create_oval(x-rad,y-rad,x+rad,y+rad,width=0,fill='green', tag=newtag)
		self.canvas.delete(oldtag)
		
