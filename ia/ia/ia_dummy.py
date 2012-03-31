


import threading
import time

import random

from .ia_base import *



class IaDummy(IaBase):
	def __init__(self, server_ip, server_port, pos_bigrobot, pos_mini_robot, pos_enemy1, pos_enemy2, *,
		canal_big_asserv, canal_mini_asserv, canal_big_others, canal_mini_others, canal_hokuyo, canal_debug
		):
		IaBase.__init__(self,
			server_ip, server_port, pos_bigrobot, pos_mini_robot, pos_enemy1, pos_enemy2,
			canal_big_asserv	=canal_big_asserv,
			canal_mini_asserv	=canal_mini_asserv,
			canal_big_others	=canal_big_others,
			canal_mini_others	=canal_mini_others,
			canal_hokuyo		=canal_hokuyo,
			canal_debug			=canal_debug
		)
		
		
	
	def loop(self):
		
		# demande de rafraichissement
		self.gamestate.ask_update()

		self.loopRobot(self.gamestate.bigrobot)
		self.loopRobot(self.gamestate.minirobot)

		time.sleep(2 + random.random() * 5.0)
		
		# attente du rafraichissement
		self.gamestate.wait_update()

	def loopRobot(self, robot):
		asserv = robot.asserv

		p = (random.randint(100,2900), random.randint(100, 1900))
		asserv.cancel()
		asserv.goto(p)
		
