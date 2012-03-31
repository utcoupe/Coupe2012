


import threading
import time

from .ia_base import *

# wait_jack -> recalage -> jouer -> wait_jack
STATE_WAIT_JACK1	= 0
STATE_RECAL			= 1
STATE_WAIT_JACK2	= 2
STATE_PLAY			= 3



class IaUtcoupe(IaBase):
	def __init__(self, server_ip, server_port, pos_bigrobot, pos_mini_robot, pos_enemy1, pos_enemy2, *,
		canal_big_asserv, canal_mini_asserv, canal_big_others, canal_mini_others, canal_hokuyo, canal_debug,
		autostart = False, match_timeout = None
		):
		"""
		@param {bool} autostart			démarrer sans attendre le signal du jack
		@param {int} match_timeout		durée d'un match en second, None pour infini
		"""
		IaBase.__init__(self,
			server_ip, server_port, pos_bigrobot, pos_mini_robot, pos_enemy1, pos_enemy2,
			canal_big_asserv	=canal_big_asserv,
			canal_mini_asserv	=canal_mini_asserv,
			canal_big_others	=canal_big_others,
			canal_mini_others	=canal_mini_others,
			canal_hokuyo		=canal_hokuyo,
			canal_debug			=canal_debug
		)

		enemies = self.gamestate.enemyrobots()
		
		# actions gros robot
		bigrobot = self.gamestate.bigrobot
		actions = get_actions_bigrobot(bigrobot, bigrobot.asserv, enemies)
		bigrobot.set_actions(actions)
		
		# actions mini robot
		minirobot = self.gamestate.minirobot
		actions = get_actions_minirobot(minirobot, minirobot.asserv, enemies)
		minirobot.set_actions(actions)

		#
		self.state_match	= STATE_WAIT_JACK1
		self.autostart		= autostart
		self.t_begin_match	= time.time()
		self.match_timeout	= match_timeout

		self.e_jack			= threading.Event()

	def loop(self):

		# compute next match state
		self.next_state()


		# appellé la fonction correspondant à l'état acutel
		if 		STATE_PLAY	== self.state_match:
			self.loopPlay()
		elif 	STATE_RECAL	== self.state_match:
			self.loopRecal()
		else:
			self.loopJack()

	def loopJack(self):
		print("attente du jack...")
		if self.autostart:
			self.on_jack_event()
		else:
			self.e_jack.wait(2)
		
			
	def loopPlay(self):
		
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

	def loopRecal(self):
		"""
		Phase de racalage du robot au début
		@todo
		"""
		self.state_match = STATE_PLAY
		
	
	def stats(self):
		time_since_last_show_stats = time.time() - self.time_last_show_stats
		if time_since_last_show_stats >= 2:
			self.print_stats()
			self.time_last_show_stats = time.time()
			print(self.gamestate.bigrobot.actions)
			print(self.gamestate.minirobot.actions)

	def next_state(self):
		if 		STATE_WAIT_JACK1		== self.state_match:
			pass
		elif 	STATE_RECAL				== self.state_match:
			pass
		elif 	STATE_WAIT_JACK2		== self.state_match:
			pass
		elif 	STATE_PLAY				== self.state_match:
			# si le match a durée depuis trop longtemps, on s'arrête
			if self.match_timeout and (time.time() - self.t_begin_match) > self.match_timeout:
				self.state_match = STATE_WAIT_JACK2

	def on_jack_event(self):
		"""
		Fonction appellé lorsque le jack est tiré
		"""
		if 		STATE_WAIT_JACK1		== self.state_match:
			self.state_match = STATE_RECAL
		elif 	STATE_WAIT_JACK2		== self.state_match:
			self.state_match = STATE_PLAY
		self.e_jack.set()
