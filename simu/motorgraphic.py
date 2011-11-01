# -*- coding: utf-8 -*-

from define import *


import pygame as pg
from pygame.locals import *
from pygame.color import *


class MotorGraphic():	
	def __init__(self):
		pg.init()
		self.map_height = 2000
		self.map_width = 3000
		self.screen = pg.display.set_mode((600, 400))
		self.clock = pg.time.Clock()
		self.objects = []
		self.onMouseEvents = []
		self.collisions_to_draw = []
		self.map_img = pg.image.load("map.png")
		self.map_img=pg.transform.scale(self.map_img,(self.screen.get_width(),self.screen.get_height()))
		print self.to_px((200,300))

	def step(self):
		### Clear screen
		self.screen.fill(THECOLORS["white"])
		
		### draw background
		self.screen.blit(self.map_img,(0,0))

		### Draw
		for obj in self.objects :
			if obj.t == CIRCLE:
				self.draw_circle(obj.shape)
			elif obj.t == POLY:
				self.draw_poly(obj.shape)

		for p,r in self.collisions_to_draw:
			pg.draw.circle(self.screen, THECOLORS["red"], p, r, 0)
		self.collisions_to_draw = []
		
		### Flip screen
		pg.display.flip()
		self.clock.tick(60)
		pg.display.set_caption("fps: " + str(self.clock.get_fps()))

		### Events
		for event in pg.event.get():
			if event.type == QUIT:
				return False
			elif event.type == KEYDOWN and event.key == K_ESCAPE:
				return False
			elif event.type == MOUSEBUTTONDOWN and event.button == 1: # LMB
				p = event.pos
				for f in self.onMouseEvents: f(p[0], p[1])

		return True

	def to_px(self, coord):
		x,y=coord
		return ((self.screen.get_width()*x)/self.map_width,(self.screen.get_height()*x)/self.map_height)
	
	def add(self, obj):
		self.objects.append(obj)
		try:
			self.onMouseEvents.append(obj.onMouseEvent)
		except:
			pass

	def draw_circle(self, circle):
		p = map(int, circle.body.position)
		pg.draw.circle(self.screen, THECOLORS["blue"], p, int(circle.radius), 2)
	
	def draw_poly(self, poly):
		body = poly.body
		ps = poly.get_points()
		ps.append(ps[0])
		pg.draw.lines(self.screen, THECOLORS["green"], False, ps)
	
	def draw_collision(self, space, arb):
		for c in arb.contacts:
			r = max( 3, abs(c.distance*5) )
			r = int(r)
			p = map(int, c.position)
			self.collisions_to_draw.append((p, r))
