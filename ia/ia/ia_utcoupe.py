


import threading
import time

from py3irc.mypyirc.ircdefine import *
from geometry import ConvexPoly

from .clientIRC import IABot, Asservissement
from .gamestate import GameState
from .robot import Robot
from .graph import NavGraph
from .debug import Debug
from .action import Action
from .actions import *



FILENAME_MAP	= "map.xml"
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


class IaUtcoupe:
	def __init__(self, server_ip, server_port, pos_bigrobot, pos_mini_robot, pos_enemy1, pos_enemy2, *,
		canal_big_asserv, canal_mini_asserv, canal_big_others, canal_mini_others, canal_hokuyo, canal_debug
		):

		# création bot irc
		self.ircbot = IABot(server_ip, server_port,
			canal_big_asserv	= canal_big_asserv,
			canal_mini_asserv	= canal_mini_asserv,
			canal_big_others	= canal_big_others,
			canal_mini_others	= canal_mini_others,
			canal_debug			= canal_debug,
			canal_hokuyo		= canal_hokuyo)
		
		# démarage du bot irc
		self.t_ircbot = threading.Thread(None, self.ircbot.start, "loop iabot")
		self.t_ircbot.setDaemon(True)
		self.t_ircbot.start()

		
		#####
		## Module de debug
		#####
		self.debug = Debug(self.ircbot, canal_debug)




		self.init_pos = {}
		self.init_pos['big'] = pos_bigrobot
		self.init_pos['mini'] = pos_mini_robot
		self.init_pos['enemy1'] = pos_enemy1
		self.init_pos['enemy2'] = pos_enemy2
		enemy1 = Robot(self.init_pos['enemy1'], None)
		enemy2 = Robot(self.init_pos['enemy1'], None)
		enemies = (enemy1, enemy2)



		#####
		## Création gros robot
		#####

		# création du graph de déplacement
		ng = NavGraph(R_BIGROBOT,FILENAME_MAP)
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy1'],200,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy2'],200,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['mini'],R_MINIROBOT,8))
		ng.update()
		
		# robot
		bigrobot = Robot(self.init_pos['big'], ng)

		# création de l'asserv
		asserv = Asservissement(self.ircbot, canal_big_asserv)
		bigrobot.set_asserv(asserv)

		# actions
		actions = get_actions_bigrobot(bigrobot, asserv, enemies)
		bigrobot.set_actions(actions)
		

		#####
		## Création mini robot
		#####
		
		# création du graph de déplacement
		ng = NavGraph(R_BIGROBOT,FILENAME_MAP)
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy1'],200,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy2'],200,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['big'],R_BIGROBOT,8))
		ng.update()

		#robot
		minirobot = Robot(self.init_pos['mini'], ng)
		
		# création de l'asserv
		asserv = Asservissement(self.ircbot, canal_mini_asserv)
		minirobot.set_asserv(asserv)

		# actions
		actions = get_actions_minirobot(minirobot, asserv, enemies)
		minirobot.set_actions(actions)



		#####
		## Gamestate
		#####
		self.gamestate = GameState(self.ircbot, canal_big_asserv, canal_mini_asserv, bigrobot, minirobot, enemy1, enemy2)


		

		#####
		## Statistiques
		#####
		self.time_last_show_stats	= 0
		self.sums = {}
		self.sums['mainloop'] = {'t':0, 'n':0}


	def reset(self):
		self.gamestate.reset()
		self.debug.reset()


	def start(self):
		print("Attente de la connection au serveur IRC...")
		self.ircbot.e_welcome.wait()
		print("Get latency big asserv")
		print(self.gamestate.bigrobot.asserv.get_latency())
		print("Get latency mini asserv")
		print(self.gamestate.bigrobot.asserv.get_latency())
		print("Ping hokuyo")
		print(self.gamestate.hokuyo.get_latency())
		#input("appuyez sur une touche pour démarrer")
		self.mainloop()

	def stop(self):
		self.ircbot.stop()
		print("Exit")

	def mainloop(self):
		self.debug.reset()
		# premier rafraichissement
		self.gamestate.ask_update()
		self.gamestate.wait_update()
		# mainloop
		while 1:
			start_main_loop = time.time()

			# demande de rafraichissement
			self.gamestate.ask_update()

			self.gamestate.update_robots()

			
			# gogogo robots !
			self.loopRobot(self.gamestate.bigrobot)
			self.loopRobot(self.gamestate.minirobot)
			
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
				
			delay = max(0.05, 0.2 - time.time() - start_main_loop)
			time.sleep(delay)
	
	def loopRobot(self, robot):

		actions = robot.actions
		ng = robot.ng
		asserv = robot.asserv
		
		if not self.gamestate.bigrobot.actions and not self.gamestate.minirobot.actions:
			print("PLUS D'ACTIONS A RÉALISER !")
			self.reset()
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
					self.debug.draw_path(best_action.path, RED, id(robot))
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
			else:
				print("No reachable actions")

	def stats(self):
		time_since_last_show_stats = time.time() - self.time_last_show_stats
		if time_since_last_show_stats >= 2:
			self.print_stats()
			self.time_last_show_stats = time.time()
			print(self.gamestate.bigrobot.actions)
			print(self.gamestate.minirobot.actions)

	def print_stats(self):
		for k, s in self.sums.items():
			if s['n'] != 0:
				print(k, s['t']/s['n'])
		self.gamestate.print_stats()


		
