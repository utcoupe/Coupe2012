


import threading
import time

from .ia_base import *

# wait_jack -> recalage -> jouer -> wait_jack
STATE_WAIT_JACK1	= 0
STATE_RECAL			= 1
STATE_WAIT_JACK2	= 2
STATE_POST_RECAL	= 3
STATE_PLAY			= 4




class IaUtcoupe(IaBase):
	def __init__(self, server_ip, server_port, pos_bigrobot, pos_mini_robot, pos_enemy1, pos_enemy2, *,
	team,
		canal_big_asserv, canal_mini_asserv,
		canal_big_others, canal_mini_others,
		canal_big_visio, canal_mini_visio,
		canal_big_extras, canal_mini_extras,
		canal_hokuyo,
		canal_debug,
		autostart = False, match_timeout = None,
		skip_recalage
		):
		"""
		@param {bool} autostart			démarrer sans attendre le signal du jack
		@param {int} match_timeout		durée d'un match en second, None pour infini
		"""
		IaBase.__init__(self,
			server_ip, server_port, pos_bigrobot, pos_mini_robot, pos_enemy1, pos_enemy2,
			team				=team,
			canal_big_asserv	=canal_big_asserv,
			canal_mini_asserv	=canal_mini_asserv,
			canal_big_others	=canal_big_others,
			canal_mini_others	=canal_mini_others,
			canal_big_visio		=canal_big_visio,
			canal_mini_visio	=canal_mini_visio,
			canal_hokuyo		=canal_hokuyo,
			canal_debug			=canal_debug,
			canal_big_extras	=canal_big_extras,
			canal_mini_extras	=canal_mini_extras,
		)

		# ne pas se recaler
		self.skip_recalage = skip_recalage

		# 
		self.autostart		= autostart
		
		# durée max du match
		self.match_timeout	= match_timeout

		
		# reset
		self.reset()
		
		



	def reset(self):
		self.gamestate.reset()
		self.ircbot.reset()
		self.state_match	= STATE_WAIT_JACK1
		self.state_mini	= 0
		self.state_big	= 0

		
		# bind de l'event jack
		self.ircbot.set_handler(ID_MSG_JACK, self._on_jack_event)

		# récupération des enemis
		enemies = self.gamestate.enemyrobots()
		
		# actions gros robot
		bigrobot = self.gamestate.bigrobot
		actions = get_actions_bigrobot(self, bigrobot, enemies)
		bigrobot.set_actions(actions)
		
		# actions mini robot
		minirobot = self.gamestate.minirobot
		actions = get_actions_minirobot(self, minirobot, enemies)
		minirobot.set_actions(actions)


		# date du début du match, pout compter les 90s
		self.t_begin_match	= time.time()
		
		# jack event
		self.e_jack			= threading.Event()

		# les actions liées aux cds trouvés grâce à la visio
		self.cd_actions = []
		
		time.sleep(0.5)
	
	def loop(self):
		# si le match a durée depuis trop longtemps, on s'arrête
		if self.match_timeout and (time.time() - self.t_begin_match) > self.match_timeout:
			self.reset()

		# appellé la fonction correspondant à l'état acutel
		if 		STATE_PLAY	== self.state_match:
			self.loopPlay()
		elif 	STATE_RECAL	== self.state_match:
			self.loopRecal()
		elif 	STATE_POST_RECAL == self.state_match:
			self.loopPostRecal()
		else:
			self.loopJack()

	def loopJack(self):
		print("attente du jack...")
		if self.autostart:
			if 		STATE_WAIT_JACK1 == self.state_match:
				self.on_jack_event(JACK_IN)
			elif 	STATE_WAIT_JACK2 == self.state_match:
				self.on_jack_event(JACK_OUT)
			else:
				self.on_jack_event(JACK_OUT)
		else:
			if self.e_jack.is_set():
				self.e_jack.clear()
			self.e_jack.wait(2)
		
			
	def loopPlay(self):
		
		for o in self.gamestate.objects:
			self.debug.remove_circle(1, id(o))
		
		# demande de rafraichissement
		self.gamestate.ask_update()

		# attente du rafraichissement
		self.gamestate.wait_update()

		# application du rafraichissement
		self.gamestate.update_robots()

		# maj des actions
		self.set_cd_actions(self.gamestate.objects)

		
		# debug
		"""
		self.debug.remove_circle(1, ID_DEBUG_ENEMY1)
		self.debug.draw_circle(self.gamestate.enemy1.pos, R_ENEMY, (0,0,255), 1, ID_DEBUG_ENEMY1)
		
		self.debug.remove_circle(1, ID_DEBUG_ENEMY2)
		self.debug.draw_circle(self.gamestate.enemy2.pos, R_ENEMY, (0,0,255), 1, ID_DEBUG_ENEMY2)
		
		self.debug.remove_circle(1, ID_DEBUG_BIGROBOT)
		self.debug.draw_circle(self.gamestate.bigrobot.pos, R_BIGROBOT, (0,255,0), 1, ID_DEBUG_BIGROBOT)
		
		self.debug.remove_circle(1, ID_DEBUG_MINIROBOT)
		self.debug.draw_circle(self.gamestate.minirobot.pos, R_MINIROBOT, (0,255,0), 1, ID_DEBUG_MINIROBOT)
		"""
		

		# voir les objets vu par les cameras
		"""for o in self.gamestate.objects:
			self.debug.draw_circle(o, 80, (255,0,0), 1, id(o))"""
		
		# gogogo robots !
		if self.gamestate.us_detect:
			print("Les pings ont detectés un bolosse en face !!!")
		elif self.gamestate.enemies_angle_mort:
			print("Un bolosse est dans l'angle mort !!!")
		else:
			self.loopRobot(self.gamestate.bigrobot)
		#self.loopRobot(self.gamestate.minirobot) # decommenter
			
		
		


	
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
			# recherche de la meilleur action à effectuer
			for action in actions:
				action.compute_score(robot.pos)
			reachable_actions = tuple(filter(lambda a: a.path, actions))
			
			if reachable_actions:
				best_action = min(reachable_actions, key=lambda a: a.score)
				
				#print(best_action)

				# si cette action n'est pas déjà celle que le robot veut atteindre
				# ou qu'un robot a coupé le chemin
				if robot.is_new_action(best_action) or \
					robot.is_path_intersected():
					print("CHANGEMENT D'ACTION", best_action)
					asserv.cancel()
					self.debug.draw_path(best_action.path, (255,0,0), id(robot))
					print(robot.pos, best_action.point_acces, best_action.path)
					robot.set_target_action(best_action, best_action.path)

				# si le robot est arrivé au point de lancement de l'action
				if robot.is_arrive():
					print("YEEEES")
					best_action.start()
				else:
					# si le robot a atteind le prochain point de passage,
					# on passe au suivant
					if robot.reach_next_checkpoint():
						goal = robot.get_next_checkpoint()
						print("goto %s" % goal)
						#self.bigrobot.cancel()
						if goal:
							asserv.goto(goal)
			else:
				print("No reachable actions")

	def mini_next_on_response_2(self, next_state):
		""" changement de l'état du petit robot quand une double réponse a été reçue """
		def __f(n, canal, args, kwargs):
			print("BOUKAKE")
			if n == 1:
				self.state_mini = next_state
		return __f
		
	def big_next_on_response_2(self, next_state):
		""" changement de l'état du gros robot quand une double réponse a été reçue """
		def __f(n, canal, args, kwargs):
			if n == 1:
				self.state_big = next_state
		return __f
		
	def loopRecal(self):
		"""
		Phase de racalage du robot au début
		"""
		print("RECALAGE", self.state_big, self.state_mini)
		
		minirobot = self.gamestate.minirobot
		bigrobot = self.gamestate.bigrobot

		# si on doit sauter le recalage (pour les tests)
		if self.skip_recalage:
			bigrobot.asserv.cancel(block=True)
			#minirobot.asserv.cancel(block=True)						// IMPORTANT A DECOMMENTER
			bigrobot.extras.teleport(self.p((120,250)), self.a(0))
			bigrobot.asserv.set_pos(self.p((120,250)), self.a(0))
			minirobot.extras.teleport(self.p((400,250)), self.a(0))
			minirobot.extras.teleport(self.p((-20,-20)), self.a(0)) # suppression du petit robot pour l'instant // IMPORTANT A DECOMMENTER
			#bigrobot.asserv.set_pos(self.p((4000,4000)), self.a(0))
			self.next_state_match()
			return
		
		if 0 == self.state_mini and \
		   0 == self.state_big:
			self.reset()
			bigrobot.asserv.cancel(block=True)
			minirobot.asserv.cancel(block=True)
			bigrobot.extras.teleport(self.p((1100,250)), self.a(90))
			minirobot.extras.teleport(self.p((250,250)), self.a(90))
			# petit et gros reculent et se calent contre le mur
			self.state_mini = 42
			self.state_big = 42
			minirobot.asserv.pwm(-100, -100, 1000, handler=self.mini_next_on_response_2(1))
			bigrobot.asserv.pwm(-100, -100, 1000, handler=self.big_next_on_response_2(1))

		if 1 == self.state_mini:
			# petit avance de 10cm
			self.state_mini = 42
			minirobot.asserv.gotor((100,0), handler=self.mini_next_on_response_2(2))

		if 1 == self.state_big:
			# gros avance de 10cm
			self.state_big = 42
			bigrobot.asserv.gotor((100,0), handler=self.big_next_on_response_2(2))
			
		if 2 == self.state_mini:
			# petit tourne de 90°
			self.state_mini = 42
			minirobot.asserv.turnr(self.a(-90), handler=self.mini_next_on_response_2(3))
			
		if 2 == self.state_big:
			# gros tourne de 90°
			self.state_big = 42
			bigrobot.asserv.turnr(self.a(-90), handler=self.big_next_on_response_2(3))
		
		if 3 == self.state_mini:
			# petit recul et se cale contre le mur
			minirobot.asserv.pwm(-100,-100, 1000, block=True, block_level=2)
			# petit avance de 10cm
			self.state_mini = 42
			minirobot.asserv.gotor((100,0), handler=self.mini_next_on_response_2(4))
		elif 4 == self.state_mini:
			# petit sort
			self.state_mini = 42
			minirobot.asserv.goto(self.p((700,250)), handler=self.mini_next_on_response_2(5))
		elif 5 == self.state_mini:
			# petit tourne
			self.state_mini = 42
			minirobot.asserv.turn(self.a(90), handler=self.mini_next_on_response_2(6))
		elif 6 == self.state_mini:
			# petit avance de 40cm
			self.state_mini = 42
			minirobot.asserv.gotor((400,0), handler=self.mini_next_on_response_2(7))

		if 3 == self.state_big and 7 == self.state_mini:
			# gros recul de 60cm
			self.state_big = 42
			bigrobot.asserv.gotor((-700,0), -500, handler=self.big_next_on_response_2(4))

		if 7 == self.state_mini and 4 == self.state_big:
			# petit recul
			self.state_mini = 42
			minirobot.asserv.goto(self.p((700,250)), handler=self.mini_next_on_response_2(8))
		elif 8 == self.state_mini:
			# petit tourne
			self.state_mini = 42
			minirobot.asserv.turn(self.a(0), handler=self.mini_next_on_response_2(9))
		elif 9 == self.state_mini:
			# petit se pause contre le gros
			self.state_mini = 42
			minirobot.asserv.goto(self.p((400,250)), handler=self.mini_next_on_response_2(10))
			
		if 4 == self.state_big:
			# gros recul et se cale contre le mur
			self.state_big = 42
			bigrobot.asserv.pwm(-100,-100, 1000, block=True, block_level=2)
			self.state_big = 5

		if 10 == self.state_mini:
			# et hop on lance les robots
			#bigrobot.asserv.cancel(block=True)
			#minirobot.asserv.cancel(block=True)
			self.state_mini = 0
			self.state_big = 0
			self.next_state_match()

	def loopPostRecal(self):
		"""
		Sortir de la zone de départ
		"""
		print("POST RECALAGE", self.state_big, self.state_mini)
		
		minirobot = self.gamestate.minirobot
		bigrobot = self.gamestate.bigrobot
		
		if 0 == self.state_mini and \
		   0 == self.state_big:
			bigrobot.asserv.cancel(block=True)
			#minirobot.asserv.cancel(block=True)		// A DECOMMENTER
			self.state_mini = 1
			self.state_big = 42

		if self.state_mini == 1:
			# avancer le petit robot
			self.state_mini = 42
			#minirobot.asserv.goto((500,250), handler=self.mini_next_on_response_2(2))		// A DECOMMENTER
			self.state_mini = 2 # supprimer
		elif self.state_mini == 2:
			self.state_mini = 42
			self.state_big = 42
			#minirobot.asserv.goto((1200,250), handler=self.mini_next_on_response_2(3))		// A DECOMMENTER
			self.state_mini = 3 # supprimer
			bigrobot.asserv.goto((700,R_BIGROBOT+50), handler=self.big_next_on_response_2(3))
		elif 3 == self.state_mini and \
			 3 == self.state_big:
			self.state_mini = 0
			self.state_big = 0
			self.next_state_match()
			
			
		
	
	def stats(self):
		time_since_last_show_stats = time.time() - self.time_last_show_stats
		if time_since_last_show_stats >= 2:
			self.print_stats()
			self.time_last_show_stats = time.time()
			print(self.gamestate.bigrobot.actions)
			print(self.gamestate.minirobot.actions)

	def next_state_match(self):
		if STATE_PLAY == self.state_match:
			self.reset()
		else:
			self.state_match += 1

	def _on_jack_event(self, n, canal, args, kwargs):
		try:
			state = int(args[0])
		except:
			print("ERROR _on_jack_event, n=%s, canal=%s, args=(%s), kargs=(%s)" % (n, canal, args, kwargs))
		else:
			self.on_jack_event(state)
		
	
	def on_jack_event(self, state):
		"""
		Fonction appellé lorsque le jack est tiré
		@param state état du jack
		"""
		# jack enfoncé au début -> recalage
		if 		STATE_WAIT_JACK1	== self.state_match \
			and JACK_IN				== state:
			self.state_match = STATE_RECAL
		# jack tiré pendant le recalage -> arret, repart au début
		elif 	STATE_RECAL				== self.state_match \
			and JACK_OUT				== state:
			self.state_match = STATE_WAIT_JACK1
		# jack tiré après le recalage -> départ
		elif 	STATE_WAIT_JACK2		== self.state_match \
			and JACK_OUT				== state:
			self.state_match = STATE_POST_RECAL
		self.e_jack.set()

	def p(self, p):
		"""
		inverse la position si le robot est rouge, à utiliser dans les
		scripts fixes
		@param {position} p
		@return inverse p
		"""
		if self.team == RED:
			return [self.x(p[0]), p[1]]
		else:
			return p
	
	def x(self, x):
		"""
		inverse X si la couleur du robot est rouge, à utiliser dans les
		scripts fixes
		@param x
		@return (3000 - x) si rouge sinon x
		"""
		if self.team == RED:
			return 3000 - x
		else:
			return x

	def a(self, a):
		"""
		inverse l'angle si la couleur du robot est rouge, à utiliser dans
		les scripts fixes
		@param {degrès} a
		@return (a + 180) si rouge sinon a
		"""
		if self.team == RED:
			if a > 0:
				return 180 - a
			else:
				return 180 + a
		else:
			return a

	def set_cd_actions(self, cds):
		"""
		@param cds {list:pair} list es positions des cds
		"""
		bigrobot = self.gamestate.bigrobot
		for actions in self.cd_actions:
			try:
				bigrobot.actions.remove(actions)
			except ValueError:
				pass
		self.cd_actions = []
		for cd in cds:
			self.cd_actions.append(ActionCd(bigrobot, self.gamestate.enemyrobots(), cd))
		bigrobot.actions.extend(self.cd_actions)
	
