# -*- coding: utf-8 -*-

import pygame as pg
from pygame.locals import *
from pygame.color import *

class MotorGraphic():
	def __init__(self):
		pg.init()
		self.screen = pg.display.set_mode((600, 400))
		self.clock = pg.time.Clock()
		self.shapes = []
		self.logo_img = pg.image.load("map.jpg")
		self.logo_img=pg.transform.scale(self.logo_img,(self.screen.get_width(),self.screen.get_height()))
		###print self.logo_img.__class__


	def step(self):

		### Clear screen
		self.screen.fill(THECOLORS["white"])
		
		### draw background
		self.screen.blit(self.logo_img,(0,0))

		### Draw
		for shape in self.shapes:
			p = map(int, shape.body.position)
			pg.draw.circle(self.screen, THECOLORS["blue"], p, int(shape.radius), 2)

		### Flip screen
		pg.display.flip()
		self.clock.tick(60)
		pg.display.set_caption("fps: " + str(self.clock.get_fps()))
		
		for event in pg.event.get():
			if event.type == QUIT:
				return False
			elif event.type == KEYDOWN and event.key == K_ESCAPE:
				return False

		return True

	def add(self, obj):
		self.shapes.append(obj.shape)

	def draw_collision(space, arb):
		for c in arb.contacts:
			r = max( 3, abs(c.distance*5) )
			r = int(r)
			p = map(int, c.position)
			pg.draw.circle(self.surface, THECOLORS["red"], p, r, 0)
