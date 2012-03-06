# -*- coding: utf-8 -*-

from pygame.locals import *

from . import robot
from ..define import *
from ..engine.engineobject import EngineObjectPoly

class BigRobot(robot.Robot):
	def __init__(self, *, canal_asserv, canal_others, posinit, team):
		if team == BLUE:
			self.mouse_button = 1 # LMB
			color = 'blue'
		else:
			self.mouse_button = 3 # RMB
			color = 'red'
		
		self.rouleau = EngineObjectPoly(
			colltype	= COLLTYPE_ROULEAU,
			offset		= mm_to_px(6,-137),
			color		= "orange",
			poly_points = map(lambda p: mm_to_px(*p),[(0,0),(138,0),(138,274),(0,274)])
		)
		
		robot.Robot.__init__(self,
			canal_asserv		= canal_asserv,
			canal_others		= canal_others,
			team				= team,
			posinit				= posinit,
			mass				= 10,
			color				= color,
			poly_points			= mm_to_px((0,0),(288,0),(288,314),(0,314)),
			custom_objects		= [self.rouleau]
		)

		self.nb_white_cds = 0
		self.nb_black_cds = 0
		self.nb_lingos = 0

	def onEvent(self, event):
		if event.type == KEYDOWN and event.key == K_LSHIFT:
			self.shift_on = True
		if event.type == KEYUP and event.key == K_LSHIFT:
			self.shift_on = False
		if not self.shift_on and event.type == MOUSEBUTTONDOWN and event.button == self.mouse_button:
			p = event.pos
			print(px_to_mm(p[0],p[1]))
			self._cmd_asserv_goto(*px_to_mm(p[0],p[1],mm_to_px(1000)), id_msg=42)

	def _cmd_others_drop(self, **kwargs):
		self.nb_white_cds = 0
		self.nb_black_cds = 0
		self.send_canal_asserv(kwargs['id_msg'], 1)
	
	def _cmd_others_vider_totem(self, **kwargs):
		pass

	def _cmd_others_is_full(self, **kwargs):
		coeff_engorgement = (self.nb_white_cds+self.nb_black_cds) * COEFF_ENGORGEMENT_CD
		coeff_engorgement += self.nb_lingos * COEFF_ENGORGEMENT_LINGO
		r = 0 if coeff_engorgement < 1 else 1
		self.send_canal_asserv(kwargs['id_msg'], r)

	def eat_cd(self, color):
		if color == 'white':
			self.nb_white_cds += 1
		elif color == 'black':
			self.nb_black_cds += 1
		else:
			print ("Couleur de Cd inconnue : %s" % color)

	def eat_lingo(self):
		self.nb_lingos += 1

