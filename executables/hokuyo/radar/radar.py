
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

	def draw_points(self, points):
		self.canvas.delete('points')

		rad = 5
		
		for x,y in points:
			x += MAX_VAL
			y = MAX_VAL - y
			x /= sizeFact
			y /= sizeFact

			self.canvas.create_oval(x-rad,y-rad,x+rad,y+rad,width=0,fill='green', tag='points')
		
