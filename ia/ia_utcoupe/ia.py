

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,".."))
sys.path.append(os.path.join(FILE_DIR,"..","..","lib"))



import threading
import time

from mypyirc.ircdefine import *

from clientIRC.iabot import *
from agents.robot import *
from gamestate import GameState
from graph.navgraph import *
from debug import *
from action import *
from actions import *



FILENAME_MAP	= "graph/map.xml"
RED				= (255,0,0)
GREEN			= (0,255,0)
BLUE			= (0,0,255)
R_ROBOT			= 200
R_ENEMY			= 200
ID_DEBUG_BIGROBOT	= 42
ID_DEBUG_MINIROBOT	= 43
ID_DEBUG_ENEMY1		= 56
ID_DEBUG_ENEMY2		= 57


class IA:
	def __init__(self, server_ip, server_port,
		canal_big_asserv, canal_mini_asserv, canal_hokuyo, canal_debug,
		pos_bigrobot, pos_mini_robot, pos_enemy1, pos_enemy2):

		# création bot irc
		self.ircbot = IABot("localhost", 6667, (canal_big_asserv, canal_mini_asserv, canal_hokuyo, canal_debug))
		
		# démarage du bot irc
		self.t_ircbot = threading.Thread(None, self.ircbot.start, "loop iabot")
		self.t_ircbot.setDaemon(True)
		self.t_ircbot.start()

		
		# création du module de débug
		self.debug = Debug(self.ircbot, canal_debug)


		# création des deux robots
		self.bigrobot = Robot(self.ircbot, canal_big_asserv)
		self.minirobot = Robot(self.ircbot, canal_mini_asserv)


		# création du gamestate
		dpos = {}
		dpos['big'] = pos_bigrobot
		dpos['mini'] = pos_mini_robot
		dpos['enemy1'] = pos_enemy1
		dpos['enemy2'] = pos_enemy2
		self.gamestate = GameState(self.ircbot, canal_big_asserv, canal_mini_asserv, dpos)


		# création du graph de déplacement
		self.ng = NavGraph(R_ROBOT)
		self.ng.load_xml(FILENAME_MAP)
		self.ng.add_dynamic_obstacle(Poly().initFromCircle(self.gamestate.enemy1.pos,200,8))
		self.ng.add_dynamic_obstacle(Poly().initFromCircle(self.gamestate.enemy2.pos,200,8))
		self.ng.update()

		self.actions = get_actions_bigrobot(self.gamestate, self.bigrobot, self.ng)


	def start(self):
		print("Attente de la connection au serveur IRC...")
		self.ircbot.event_on_connect.wait()
		print("Ping big asserv")
		print(self.gamestate.ping(CANAL_BIG_ASSERV))
		print("Ping mini asserv")
		print(self.gamestate.ping(CANAL_MINI_ASSERV))
		print("Ping hokuyo")
		print(self.gamestate.ping(CANAL_HOKUYO))
		input("appuyez sur une touche pour démarrer")
		self.mainloop()


	def mainloop(self):
		while 1:
			start = time.time()

			# demande de rafraichissement
			self.gamestate.ask_update()

			self.ng.dynamic_obstacles[0].move_to(self.gamestate.enemy1.pos)
			self.ng.dynamic_obstacles[1].move_to(self.gamestate.enemy2.pos)
			self.ng.update()

			if not self.actions:
				print("PLUS D'ACTIONS A RÉALISER !")
				self.actions = get_actions_bigrobot(self.gamestate, self.bigrobot, self.ng)
			
			# si le robot n'est pas en action et qu'il reste des actions
			if not self.gamestate.bigrobot.in_action and self.actions:
				# recherche de la mailleur action à effectuer
				for action in self.actions:
					action.compute_score(self.gamestate.bigrobot.pos)
				reachable_actions = tuple(filter(lambda a: a.path, self.actions))
				
				if reachable_actions:
					best_action = min(reachable_actions, key=lambda a: a.score)
					
					print(best_action)

					# si cette action n'est pas déjà celle que le robot veut atteindre
					if self.gamestate.bigrobot.is_new_action(best_action):
						print("CHANGEMENT D'ACTION")
						self.debug.draw_path(best_action.path, RED)
						print(self.gamestate.bigrobot.pos, best_action.point_acces, best_action.path)
						self.gamestate.bigrobot.set_target_action(best_action, best_action.path)

					# si le robot est arrivé
					if self.gamestate.bigrobot.is_arrive():
						print("YEEEES")
						self.actions.remove(best_action)
					else:
						# si le robot a atteind le prochain point de passage,
						# on passe au suivant
						if self.gamestate.bigrobot.reach_next_checkpoint():
							goal = self.gamestate.bigrobot.get_next_checkpoint()
							print("goto %s" % goal)
							self.bigrobot.cancel()
							if goal:
								self.bigrobot.goto(goal, 800)

			# debug
			"""self.debug.remove_circle(ID_DEBUG_ENEMY1)
			self.debug.draw_circle(self.gamestate.enemy1.pos, R_ENEMY, BLUE, ID_DEBUG_ENEMY1)
			
			self.debug.remove_circle(ID_DEBUG_ENEMY2)
			self.debug.draw_circle(self.gamestate.enemy2.pos, R_ENEMY, BLUE, ID_DEBUG_ENEMY2)"""
			
			"""self.debug.remove_circle(ID_DEBUG_BIGROBOT)
			self.debug.draw_circle(self.gamestate.bigrobot.pos, R_ENEMY, RED, ID_DEBUG_BIGROBOT)
			
			self.debug.remove_circle(ID_DEBUG_MINIROBOT)
			self.debug.draw_circle(self.gamestate.minirobot.pos, R_ENEMY, RED, ID_DEBUG_MINIROBOT)"""

			# attente du rafraichissement
			self.gamestate.wait_update()


			# calcul du temps passé
			time_ellapsed = time.time() - start
			print(time_ellapsed)
			delay = max(0.001, 0.2 - time_ellapsed)
			time.sleep(delay)


