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
			offset		= mm_to_px(0,-85),
			color		= "orange",
			poly_points = map(lambda p: mm_to_px(*p),[(0,0),(170,0),(170,170),(0,170)])
		)
		
		robot.Robot.__init__(self,
			canal_asserv		= canal_asserv,
			canal_others		= canal_others,
			team				= team,
			posinit				= posinit,
			10,
			color,
			mm_to_px((100,0),(160,0),(280,90),(280,230),(160,320),(100,320),(0,290),(0,30)),
			[self.rouleau]
		)
		
		# rajouts des fonctions utilisées par le bot irc
		# _cmd_asserv_<cmd> => cmd_<canal_asserv>_<cmd>
		self.transform("others", canal_others)

		self.nb_white_cds = 0
		self.nb_black_cds = 0
		self.nb_lingots = 0

	def onEvent(self, event):
		if event.type == KEYDOWN and event.key == K_LSHIFT:
			self.shift_on = True
		if event.type == KEYUP and event.key == K_LSHIFT:
			self.shift_on = False
		if not self.shift_on and event.type == MOUSEBUTTONDOWN and event.button == self.mouse_button:
			p = event.pos
			print(px_to_mm(p[0],p[1]))
			self._cmd_asserv_goto(*px_to_mm(p[0],p[1],mm_to_px(1000)), id_msg=42)

	def _cmd_others_drop(self):
		pass
	
	def _cmd_others_vider_totem(self):
		pass

	def _cmd_others_is_full(self):
		

	def eat_cd(self, color):
		if color == 'white':
			self.nb_white_cds += 1
		elif color == 'black':
			self.nb_black_cds += 1
		else:
			print ("Couleur de Cd inconnue : %s" % color)

