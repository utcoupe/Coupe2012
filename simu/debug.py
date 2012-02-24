# -*- coding: utf-8 -*-



import sys
import os
sys.path.append(os.path.join("..","lib"))

from mypyirc.mypyircbot import *

from define import *


class ShapeCircle:
	def __init__(self, p, radius, r, g, b):
		self.position = p
		self.radius = radius
		self.color = (r,g,b,255)

class ShapeSegment:
	def __init__(self, p1, p2, r, g, b):
		self.p1 = p1
		self.p2 = p2
		self.color = (r,g,b,255)

class Debug(Executer):
	def __init__(self):
		Executer.__init__(self)

		self.circles = {}
		self.segments = {}

		self.lock = threading.Lock()
		
	def cmd_debug_draw_circle(self, x, y, radius, r, g, b, i, **kwargs):
		o = ShapeCircle(mm_to_px(x,y),mm_to_px(radius), r, g, b)
		self.lock.acquire()
		self.circles[i] = o
		self.lock.release()

	def cmd_debug_draw_segment(self, x1, y1, x2, y2, r, g, b, i, **kwargs):
		o = ShapeSegment(mm_to_px(x1,y1), mm_to_px(x2,y2), r, g, b)
		self.lock.acquire()
		self.segments[i] = o
		self.lock.release()

	def cmd_debug_remove_circle(self, i, **kwargs):
		if i in self.circles:
			self.lock.acquire()
			self.circles.pop(i)
			self.lock.release()

	def cmd_debug_remove_segment(self, i, **kwargs):
		if i in self.segments:
			self.lock.acquire()
			self.segments.pop(i)
			self.lock.release()
	
	def cmd_debug_clear_all(self, **kwargs):
		self.circles = {}
		self.segments = {}

	def cmd_debug_reset(self, **kwargs):
		self.cmd_debug_clear_all()


	
		
