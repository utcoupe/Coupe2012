

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
from gamestate import *
from graph.navgraph import *
from debug import *
from action import *
from actions import *



FILENAME_MAP	= "graph/map.xml"
RED				= (255,0,0)
GREEN			= (0,255,0)
BLUE			= (0,0,255)
R_BIGROBOT		= 200
R_MINIROBOT		= 200
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


		self.init_pos = {}
		self.init_pos['big'] = pos_bigrobot
		self.init_pos['mini'] = pos_mini_robot
		self.init_pos['enemy1'] = pos_enemy1
		self.init_pos['enemy2'] = pos_enemy2
		enemy1 = RobotState(self.init_pos['enemy1'], None)
		enemy2 = RobotState(self.init_pos['enemy1'], None)
		enemies = (enemy1, enemy2)

		# création du graph de déplacement
		ng = NavGraph(R_BIGROBOT)
		ng.load_xml(FILENAME_MAP)
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy1'],200,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy2'],200,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['mini'],R_MINIROBOT,8))
		ng.update()

		# robot
		bigrobot = RobotState(self.init_pos['big'], ng)
		
		# création du graph de déplacement
		ng = NavGraph(R_BIGROBOT)
		ng.load_xml(FILENAME_MAP)
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy1'],200,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy2'],200,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['big'],R_BIGROBOT,8))
		ng.update()

		#robot
		minirobot = RobotState(self.init_pos['mini'], ng)

		# création du gamestate
		self.gamestate = GameState(self.ircbot, canal_big_asserv, canal_mini_asserv, bigrobot, minirobot, enemy1, enemy2)
		
		# actions
		actions = get_actions_bigrobot(bigrobot, self.bigrobot, enemies)
		bigrobot.set_actions(actions)
		actions = get_actions_minirobot(minirobot, self.minirobot, enemies)
		minirobot.set_actions(actions)

		# sert à faire les statistiques
		self.time_last_show_stats	= 0
		self.sums = {}
		self.sums['mainloop'] = {'t':0, 'n':0}
		self.sums['update_big_ng'] = {'t':0, 'n':0}
		self.sums['update_mini_ng'] = {'t':0, 'n':0}


	def reset(self):
		self.gamestate.reset()


	def start(self):
		print("Attente de la connection au serveur IRC...")
		self.ircbot.event_on_connect.wait()
		print("Ping big asserv")
		print(self.gamestate.ping(CANAL_BIG_ASSERV))
		print("Ping mini asserv")
		print(self.gamestate.ping(CANAL_MINI_ASSERV))
		print("Ping hokuyo")
		print(self.gamestate.ping(CANAL_HOKUYO))
		#input("appuyez sur une touche pour démarrer")
		self.mainloop()


	def mainloop(self):
		while 1:
			start_main_loop = time.time()

			# demande de rafraichissement
			self.gamestate.ask_update()

			self.gamestate.update_robots()

			if self.gamestate.bigrobot.is_path_intersected():
				start_update_ng = time.time()
				self.gamestate.bigrobot.ng.update()
				self.sums['update_big_ng']['t'] += time.time() - start_update_ng
				self.sums['update_big_ng']['n'] += 1
				
			if self.gamestate.minirobot.is_path_intersected():
				start_update_ng = time.time()
				self.gamestate.minirobot.ng.update()
				self.sums['update_mini_ng']['t'] += time.time() - start_update_ng
				self.sums['update_mini_ng']['n'] += 1



			
			# gogogo robots !
			robot = self.gamestate.bigrobot
			asservissement = self.bigrobot
			self.loopRobot(robot, asservissement)
			
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


			# calcul du temps écoulé
			self.sums['mainloop']['t'] += time.time() - start_main_loop
			self.sums['mainloop']['n'] += 1

			self.stats()
				
			delay = max(0.001, 0.2 - time.time() - start_main_loop)
			time.sleep(delay)
	
	def loopRobot(self, robot, asserv):

		actions = robot.actions
		ng = robot.ng
		
		if not actions:
			print("PLUS D'ACTIONS A RÉALISER !")
			self.reset()
			print (self.gamestate.bigrobot.actions)
			return
		
		# si le robot n'est pas en action et qu'il reste des actions
		if not robot.in_action and actions:
			# recherche de la mailleur action à effectuer
			for action in actions:
				action.compute_score(robot.pos)
			reachable_actions = tuple(filter(lambda a: a.path, actions))
			
			if reachable_actions:
				best_action = min(reachable_actions, key=lambda a: a.score)
				
				#print(best_action)

				# si cette action n'est pas déjà celle que le robot veut atteindre
				if robot.is_new_action(best_action):
					print("CHANGEMENT D'ACTION")
					asserv.cancel()
					self.debug.draw_path(best_action.path, RED)
					print(robot.pos, best_action.point_acces, best_action.path)
					robot.set_target_action(best_action, best_action.path)

				# si le robot est arrivé
				if robot.is_arrive():
					print("YEEEES")
					actions.remove(best_action)
				else:
					# si le robot a atteind le prochain point de passage,
					# on passe au suivant
					if robot.reach_next_checkpoint():
						goal = robot.get_next_checkpoint()
						print("goto %s" % goal)
						#self.bigrobot.cancel()
						if goal:
							asserv.goto(goal, 800)

	def stats(self):
		time_since_last_show_stats = time.time() - self.time_last_show_stats
		if time_since_last_show_stats >= 2:
			for k, s in self.sums.items():
				if s['n'] != 0:
					print(k, s['t']/s['n'])
			self.time_last_show_stats = time.time()
				


		
