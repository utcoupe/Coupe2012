


import threading
import time

import random

from .ia_base import *



class IaDummy(IaBase):
	def __init__(self, server_ip, server_port, pos_bigrobot, pos_mini_robot, pos_enemy1, pos_enemy2, *,
		team,
		canal_big_asserv, canal_mini_asserv,
		canal_big_others, canal_mini_others,
		canal_big_extras, canal_mini_extras,
		canal_hokuyo, canal_debug, skip_recalage,
		autostart = False, match_timeout = None
		):
		IaBase.__init__(self,
			server_ip, server_port, pos_bigrobot, pos_mini_robot, pos_enemy1, pos_enemy2,
			team				=team,
			canal_big_asserv	=canal_big_asserv,
			canal_mini_asserv	=canal_mini_asserv,
			canal_big_others	=canal_big_others,
			canal_mini_others	=canal_mini_others,
			canal_hokuyo		=canal_hokuyo,
			canal_debug			=canal_debug,
			canal_big_extras	=canal_big_extras,
			canal_mini_extras	=canal_mini_extras,
			canal_mini_actions	=canal_mini_actions,
			canal_big_actions	=canal_big_actions
		)
		
		
	
	def loop(self):
		
		# demande de rafraichissement
		self.gamestate.ask_update()

		self.loopRobot(self.gamestate.bigrobot)
		self.loopRobot(self.gamestate.minirobot)
		self.gamestate.minirobot.actionneurs.arracher_carte()
		#time.sleep(2 + random.random() * 5.0)
		time.sleep(2)

		# attente du rafraichissement
		self.gamestate.wait_update()

	def loopRobot(self, robot):
		asserv = robot.asserv

		p = (random.randint(100,2900), random.randint(100, 1900))
		#asserv.cancel()
		#asserv.goto(p)
		robot.extras.teleport(p, 0)
		
