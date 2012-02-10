# -*- coding: utf-8 -*-

from define import *


import pygame as pg
from pygame.locals import *
from pygame.color import *


class MotorGraphic():	
	def __init__(self):
		pg.init()
		self.screen = pg.display.set_mode(mm_to_px(3000, 2000))
		self.clock = pg.time.Clock()
		self.objects = []
		self.onEvents = []
		self.collisions_to_draw = []
		self.map_img = pg.image.load("map.jpg")
		self.map_img=pg.transform.scale(self.map_img,(self.screen.get_width(),self.screen.get_height()))

	def draw_obj(self, obj):
		if obj.t == CIRCLE:
			self.draw_circle(obj.shape, THECOLORS[obj.color])
		elif obj.t == POLY:
			self.draw_poly(obj.shape, THECOLORS[obj.color])
		elif obj.t == WALL:
			self.draw_segment(obj.shape, THECOLORS[obj.color])
		else:
			raise Exception("MotorGraphic.draw_obj : type '%s' doesn't exist"%obj.t)
		for o in obj.custom_objects:
			self.draw_obj(o)

	def step(self):
		### Clear screen
		self.screen.fill(THECOLORS["white"])
		
		### draw background
		self.screen.blit(self.map_img,(0,0))

		### Draw
		for obj in self.objects :
			self.draw_obj(obj)

		### Draw collisions
		for p,r in self.collisions_to_draw:
			pg.draw.circle(self.screen, THECOLORS["red"], p, r, 0)
		self.collisions_to_draw = []
		
		### Flip screen
		pg.display.flip()
		self.clock.tick(FPS)
		pg.display.set_caption( "fps: " + str(self.clock.get_fps()))

		### Events
		for event in pg.event.get():
			if event.type == QUIT:
				return False
			elif event.type == KEYDOWN and event.key == K_ESCAPE:
				return False
			else:
				for f in self.onEvents: f(event)

		return True
	
	def add(self, obj):
		self.objects.append(obj)
		try:
			self.onEvents.append(obj.onEvent)
		except:
			pass

	def draw_circle(self, shape, color):
		p = tuple(map(int, shape.body.position))
		pg.draw.circle(self.screen, color, p, int(shape.radius), 0)
	
	def draw_poly(self, shape, color):
		body = shape.body
		ps = shape.get_points()
		ps.append(ps[0])
		pg.draw.polygon(self.screen, color, ps, 0)

	def draw_segment(self, shape, color):
		body = shape.body
		p1 = body.position + shape.a.rotated(body.angle)
		p2 = body.position + shape.b.rotated(body.angle)
		pg.draw.lines(self.screen, color, False, [p1,p2])
	
	def draw_collision(self, space, arb):
		for c in arb.contacts:
			r = 5 #min(10, max( 3, abs(c.distance*5) ))
			r = int(r)
			p = tuple(map(int, c.position))
			self.collisions_to_draw.append((p, r))


	def remove(self, obj):
		self.objects.remove(obj)
	
