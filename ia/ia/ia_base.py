


import threading
import time

from py3irc.mypyirc.ircdefine import *
from geometry import ConvexPoly

from .clientIRC import *
from .gamestate import GameState
from .robot import Robot
from .graph import NavGraph
from .debug import Debug
from .action import Action
from .actions import *
from .define import *



class IaBase:
	def __init__(self, server_ip, server_port, pos_bigrobot, pos_mini_robot, pos_enemy1, pos_enemy2, *,
		team,
		canal_big_asserv, canal_mini_asserv,
		canal_big_others, canal_mini_others,
		canal_big_visio, canal_mini_visio,
		canal_big_extras, canal_mini_extras,
		canal_hokuyo, canal_debug
		):
		"""
		@param {str} server_ip
		@param {int} server_port
		@param {int} team RED|BLUE
		@param {pair} pos_bigrobot				position de départ du gros robot
		@param {pair} pos_mini_robot			position de départ du petit robot
		@param {pair} pos_enemy1				position de l'ennemi 1
		@param {pair} pos_enemy2				position de l'ennemi 2
		@param {str} canal_big_asserv
		@param {str} canal_mini_asserv
		@param {str} canal_big_others
		@param {str} canal_mini_others
		@param {str} canal_hokuyo
		@param {str} canal_debug
		@param {str} canal_big_extras
		@param {str} canal_mini_extras
		"""
		# création bot irc
		self.ircbot = IABot(server_ip, server_port, [
			canal_big_asserv,
			canal_mini_asserv,
			canal_big_others,
			canal_mini_others,
			canal_big_visio,
			canal_mini_visio,
			canal_debug,
			canal_hokuyo,
			canal_big_extras,
			canal_mini_extras
			]
		)
		
		# démarage du bot irc
		self.t_ircbot = threading.Thread(None, self.ircbot.start, "loop iabot")
		self.t_ircbot.setDaemon(True)
		self.t_ircbot.start()

		# équipe
		self.team = team

		
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
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy1'],R_ENEMY,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy2'],R_ENEMY,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['mini'],R_MINIROBOT,8))
		ng.update()
		
		# robot
		bigrobot = Robot(self.init_pos['big'], ng)

		# création de l'asserv
		asserv = Asservissement(self.ircbot, canal_big_asserv)
		bigrobot.set_asserv(asserv)

		# création de la visio
		visio = Visio(self.ircbot, canal_big_visio)
		bigrobot.set_visio(visio)
		
		# extras
		extras = Extras(self.ircbot, canal_big_extras)
		bigrobot.set_extras(extras)
		

		#####
		## Création mini robot
		#####
		
		# création du graph de déplacement
		ng = NavGraph(R_MINIROBOT,FILENAME_MAP)
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy1'],R_ENEMY,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['enemy2'],R_ENEMY,8))
		ng.add_dynamic_obstacle(ConvexPoly().initFromCircle(self.init_pos['big'],R_BIGROBOT,8))
		ng.update()

		#robot
		minirobot = Robot(self.init_pos['mini'], ng)
		
		# création de l'asserv
		asserv = Asservissement(self.ircbot, canal_mini_asserv)
		minirobot.set_asserv(asserv)
		
		#creation des actionneurs
		petits_actionneurs = ActionneursMini(self.ircbot, canal_mini_others)
		minirobot.set_actionneurs(petits_actionneurs)

		# extras
		extras = Extras(self.ircbot, canal_mini_extras)
		minirobot.set_extras(extras)

		#####
		## Gamestate
		#####
		self.gamestate = GameState(self.ircbot, canal_big_asserv, canal_mini_asserv, canal_big_visio, canal_mini_visio, bigrobot, minirobot, enemy1, enemy2)


		

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
		#print("Get latency mini asserv")
		#print(self.gamestate.bigrobot.asserv.get_latency()) 			# a DECOMMENTER
		print("Get latency big visio")
		print(self.gamestate.bigrobot.visio.get_latency())
		#print("Get latency mini visio")
		#print(self.gamestate.minirobot.visio.get_latency())			# a DECOMMENTER
		#print("Ping hokuyo")
		#print(self.gamestate.hokuyo.get_latency()) 					# a DECOMMENTER
		#input("appuyez sur une touche pour démarrer")
		self.loopsetup()
		while 1:
			start_main_loop = time.time()

			# faire quelquechose
			self.loop()
			
			# calcul du temps écoulé
			self.sums['mainloop']['t'] += time.time() - start_main_loop
			self.sums['mainloop']['n'] += 1

			# calcul des stats
			self.stats()
				
			delay = max(0.05, 0.2 - time.time() - start_main_loop)
			time.sleep(delay)

	def stop(self):
		self.ircbot.stop()
		print("Exit")

	def loopsetup(self):
		self.debug.reset()
		# premier rafraichissement
		self.gamestate.ask_update()
		self.gamestate.wait_update()
	
	def loop(self):

		# demande de rafraichissement
		self.gamestate.ask_update()
		
		# attente du rafraichissement
		self.gamestate.wait_update()


	def stats(self):
		time_since_last_show_stats = time.time() - self.time_last_show_stats
		if time_since_last_show_stats >= 2:
			self.print_stats()
			self.time_last_show_stats = time.time()

	def print_stats(self):
		for k, s in self.sums.items():
			if s['n'] != 0:
				print(k, s['t']/s['n'])
		self.gamestate.print_stats()


		

