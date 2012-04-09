# -*- coding: utf-8 -*-


import threading

from py3irc.mypyirc.mypyircbot import Executer

from .define import *


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
		
	def cmd_debug_draw_circle(self, x, y, radius, r, g, b, group, i, **kwargs):
		o = ShapeCircle(mm_to_px(x,y),mm_to_px(radius), r, g, b)
		self.lock.acquire()
		if group not in self.circles:
			self.circles[group] = {}
		self.circles[group][i] = o
		self.lock.release()

	def cmd_debug_draw_segment(self, x1, y1, x2, y2, r, g, b, group, i, **kwargs):
		o = ShapeSegment(mm_to_px(x1,y1), mm_to_px(x2,y2), r, g, b)
		self.lock.acquire()
		if group not in self.segments:
			self.segments[group] = {}
		self.segments[group][i] = o
		self.lock.release()

	def cmd_debug_remove_circle(self, group, i, **kwargs):
		if group in self.circles:
			if i in self.circles[group]:
				self.lock.acquire()
				self.circles[group].pop(i)
				self.lock.release()

	def cmd_debug_remove_segment(self, group, i, **kwargs):
		if group in self.segments:
			if i in self.segments[group]:
				self.lock.acquire()
				self.segments[group].pop(i)
				self.lock.release()

	def cmd_debug_remove_circle_group(self, group, **kwargs):
		if group in self.circles:
			self.lock.acquire()
			self.circles.pop(group)
			self.lock.release()

	def cmd_debug_remove_segment_group(self, group, **kwargs):
		if group in self.segments:
			self.lock.acquire()
			self.segments.pop(group)
			self.lock.release()
	
	def cmd_debug_clear_all(self, **kwargs):
		self.lock.acquire()
		self.circles.clear()
		self.segments.clear()
		self.lock.release()

	def cmd_debug_reset(self, **kwargs):
		self.cmd_debug_clear_all()


	
		
