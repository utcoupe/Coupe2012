

import sys
sys.path.append("geometry")
from geometry.vec import *
from geometry.circle import *
from geometry.line import *

import tkinter as tk

WIDTH = 600
HEIGHT = 400

class View(tk.Tk):
	def __init__(self):
		tk.Tk.__init__(self,None)
		self.canvas = tk.Canvas(self, width=WIDTH, height=HEIGHT, background='white')
		self.canvas.pack()
		self.w_to_px = WIDTH/3000
		self.h_to_px = HEIGHT/2000
		self.canvas.bind('<Button-1>',self.onLeft)
		self.canvas.bind('<B1-Motion>',self.onLeft)
		self.canvas.bind('<Button-3>',self.onRight)
		self.canvas.bind('<B3-Motion>',self.onRight)
		self.canvas.bind('<Button-2>',self.onWheel)
		self.canvas.bind('<B2-Motion>',self.onWheel)

		self.pos_hokyo = Vec2(0,2000)

		self.circleG = None
		self.circleR = None
		self.lineG1 = None
		self.lineG2 = None
		self.lineR1 = None
		self.lineR2 = None
		
	def onWheel(self, event):
		p = self.event_to_x_y(event)
		print(p)
		R = 40
		circle = Circle(p,R)
		T1 = self.pos_hokyo.tangente(circle,1)
		T2 = self.pos_hokyo.tangente(circle,-1)
		self.canvas.delete(self.lineR1)
		self.canvas.delete(self.lineR2)
		self.canvas.delete(self.circleR)
		p11 = T1.B
		p12 = T1.pointFrom(1000000)
		p21 = T2.B
		p22 = T2.pointFrom(1000000)
		p11 = self.to_px(p11)
		p12 = self.to_px(p12)
		p21 = self.to_px(p21)
		p22 = self.to_px(p22)
		self.circleR = self.canvas.create_oval(event.x - R * self.w_to_px, event.y - R * self.h_to_px, event.x + R * self.w_to_px, event.y + R * self.h_to_px)
		self.lineR1 = self.canvas.create_line(p11.x, p11.y, p12.x, p12.y)
		self.lineR2 = self.canvas.create_line(p21.x, p21.y, p22.x, p22.y)

	def onLeft(self, event):
		p = self.event_to_x_y(event)
		print(p)
		R = 40
		circle = Circle(p,R)
		T1 = self.pos_hokyo.tangente(circle,1)
		T2 = self.pos_hokyo.tangente(circle,-1)
		self.canvas.delete(self.lineG1)
		self.canvas.delete(self.lineG2)
		self.canvas.delete(self.circleG)
		p11 = T1.B
		p12 = T1.pointFrom(1000000)
		p21 = T2.B
		p22 = T2.pointFrom(1000000)
		p11 = self.to_px(p11)
		p12 = self.to_px(p12)
		p21 = self.to_px(p21)
		p22 = self.to_px(p22)
		self.circleG = self.canvas.create_oval(event.x - R * self.w_to_px, event.y - R * self.h_to_px, event.x + R * self.w_to_px, event.y + R * self.h_to_px)
		self.lineG1 = self.canvas.create_line(p11.x, p11.y, p12.x, p12.y)
		self.lineG2 = self.canvas.create_line(p21.x, p21.y, p22.x, p22.y)
		

	def onRight(self, event):
		p = self.event_to_x_y(event)
		print(p)
		

	def onRight(self, event):
		p = self.event_to_x_y(event)
		
	def event_to_x_y(self, event):
		return Vec2(round(event.x / self.w_to_px), round((HEIGHT - event.y) / self.h_to_px))

	def to_px(self, v):
		return Vec2(round(v.x * self.w_to_px), HEIGHT - round(v.y * self.h_to_px))


v = View()
v.mainloop()
