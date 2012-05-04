


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","lib"))


import re
import time
import threading
from inspect import currentframe
import itertools


from py3irc.mypyirc.ircdefine import *
from .clientIRC.hokuyo import Hokuyo
from .clientIRC.visio import Visio




# petite optimisation pour le calcul de k parmis 4, seul les valeurs de
# l'interval [0;4] sont calculées
C_K_PARMIS_4 = list(( tuple(itertools.permutations(range(4), i)) for i in range(5) ))
def permutation_k_parmis_4(k):
	return C_K_PARMIS_4[k]




class GameState:
	def __init__(self, ircbot, canal_big_asserv, canal_mini_asserv, canal_big_visio, canal_mini_visio, bigrobot, minirobot, enemy1, enemy2):
		self.ircbot 			= ircbot
		self.canal_big_asserv 	= canal_big_asserv
		self.canal_mini_asserv 	= canal_mini_asserv
		self.canal_big_visio 	= canal_big_visio
		self.canal_mini_visio 	= canal_mini_visio
		self.bigrobot 			= bigrobot
		self.minirobot 			= minirobot
		self.enemy1		 		= enemy1
		self.enemy2			 	= enemy2

		self.hokuyo				= Hokuyo(ircbot, CANAL_HOKUYO)

		self.objects			= []

		self.event_bigrobot_pos_update = threading.Event()
		self.event_minirobot_pos_update = threading.Event()
		self.event_hokuyo_update = threading.Event()
		self.event_on_pong = threading.Event()
		self.event_bigrobot_visio_update = threading.Event()
		self.event_minirobot_visio_update = threading.Event()

		

		self.sums = {}
		self.sums['update_big_ng'] = {'t':0, 'n':0}
		self.sums['update_mini_ng'] = {'t':0, 'n':0}
			

	def reset(self):
		self.bigrobot.reset()
		self.minirobot.reset()
		self.enemy1.reset()
		self.enemy2.reset()
		
		self.bigrobot.ng.dynamic_obstacles[0].move_to(self.enemy1.pos)
		self.bigrobot.ng.dynamic_obstacles[1].move_to(self.enemy2.pos)
		self.bigrobot.ng.dynamic_obstacles[2].move_to(self.minirobot.pos)
		self.bigrobot.ng.update()
		self.minirobot.ng.dynamic_obstacles[0].move_to(self.enemy1.pos)
		self.minirobot.ng.dynamic_obstacles[1].move_to(self.enemy2.pos)
		self.minirobot.ng.dynamic_obstacles[2].move_to(self.bigrobot.pos)
		self.minirobot.ng.update()

		
		self.sums = {}
		self.sums['update_big_ng'] = {'t':0, 'n':0}
		self.sums['update_mini_ng'] = {'t':0, 'n':0}


	def update_robots(self):
		self.bigrobot.ng.dynamic_obstacles[0].move_to(self.enemy1.pos)
		self.bigrobot.ng.dynamic_obstacles[1].move_to(self.enemy2.pos)
		self.bigrobot.ng.dynamic_obstacles[2].move_to(self.minirobot.pos)
		self.minirobot.ng.dynamic_obstacles[0].move_to(self.enemy1.pos)
		self.minirobot.ng.dynamic_obstacles[1].move_to(self.enemy2.pos)
		self.minirobot.ng.dynamic_obstacles[2].move_to(self.bigrobot.pos)
		
		#if self.bigrobot.is_path_intersected():
		start_update_ng = time.time()
		self.bigrobot.ng.update()
		self.sums['update_big_ng']['t'] += time.time() - start_update_ng
		self.sums['update_big_ng']['n'] += 1
			
		#if self.minirobot.is_path_intersected():
		start_update_ng = time.time()
		self.minirobot.ng.update()
		self.sums['update_mini_ng']['t'] += time.time() - start_update_ng
		self.sums['update_mini_ng']['n'] += 1
		
	
	def ask_update(self):
		self.event_bigrobot_pos_update.clear()
		self.event_minirobot_pos_update.clear()
		self.event_hokuyo_update.clear()
		self.event_bigrobot_visio_update.clear()
		self.event_minirobot_visio_update.clear()
		
		
		self.ask_asserv_for_pos(self.bigrobot)
		#self.ask_asserv_for_pos(self.minirobot) 						// IMPORTANT A DECOMMENTER
		self.ask_visio_for_objects()
		#self.ask_hokyo_for_pos()										// IMPORTANT A DECOMMENTER

		
	def wait_update(self):
		self.event_bigrobot_pos_update.wait()
		self.event_bigrobot_visio_update.wait()
		#self.event_minirobot_visio_update.wait()						// IMPORTANT A DECOMMENTER
		#self.event_minirobot_pos_update.wait() 						// IMPORTANT A DECOMMENTER
		#self.event_hokuyo_update.wait() 								// IMPORTANT A DECOMMENTER

	def ping(self, canal):
		n = 10
		start = time.time()
		for i in range(n):
			self.event_on_pong.clear()
			self.ircbot.send(canal, PREFIX_CMD+"ping # id=%s" % ID_MSG_PING)
			self.event_on_pong.wait()
		return (time.time() - start) / n
		

	def ask_hokyo_for_pos(self):
		self.hokuyo.get(handler=self.on_msg_hokyo)

	def ask_asserv_for_pos(self, robot):
		robot.asserv.get_pos(handler=self.on_msg_pos)

	def ask_visio_for_objects(self):
		self.objects = []
		self.bigrobot.visio.get(handler=self.on_msg_visio)
		#self.minirobot.visio.get(handler=self.on_msg_visio) 			// IMPORTANT A DECOMMENTER
	
	def on_msg(self, canal, auteur, msg):
		msg_split = msg.split(SEP)
		if len(msg_split) > 1:
			id_msg = int(msg_split[0])
			params = msg_split[1:]
			if ID_MSG_PING == id_msg:
				self.event_on_pong.set()
			elif ID_MSG_HOKUYO == id_msg:
				self.on_msg_hokyo(params)
			elif ID_MSG_POS == id_msg:
				self.on_msg_pos(canal,params)

	def on_msg_pos(self, n, canal, args, options):
		if len(args) >= 3:
			# transformation des strings en int
			args = tuple(map(int, args))
			# choix du robot à update
			if canal == self.canal_big_asserv:
				robot_to_update = self.bigrobot
				self.event_bigrobot_pos_update.set()
			elif canal == self.canal_mini_asserv:
				robot_to_update = self.minirobot
				self.event_minirobot_pos_update.set()
			else:
				print("Error %s.on_msg_pos (%s:%d) : canal non connu : %s " % (self.__class__.__name__, currentframe().f_code.co_filename, currentframe().f_lineno, canal))
			# update
			robot_to_update.update_pos(args[0:2])
			robot_to_update.a = args[2]
		else:
			print("Error %s.on_msg_pos (%s:%d) : pas assez de paramètres " %
				(self.__class__.__name__, currentframe().f_code.co_filename, currentframe().f_lineno))
	

	def on_msg_hokyo(self, n, canal, args, options):
		if len(args) == 1:
			lpos = eval(args[0])
			robots = self.robots()
			def test_permut(permut):
				l = ( (robots[j].pos - lpos[i]).norm2() for i,j in enumerate(permut) )
				return sum(l)
			best_permut = min(permutation_k_parmis_4(len(lpos)), key=lambda permut: test_permut(permut))
			for i,j in enumerate(best_permut):
				if robots[j] in self.enemyrobots():
					robots[j].update_pos(lpos[i])
			self.event_hokuyo_update.set()
		else:
			self.send_error(canal, "Error %s.on_msg_hokyo (%s:%d) : pas assez de paramètres " %
				(self.__class__.__name__, currentframe().f_code.co_filename, currentframe().f_lineno))

	def on_msg_visio(self, n, canal, args, options):
		if len(args) == 2:
			if canal == self.canal_big_visio:
				print("coucou")
				event = self.event_bigrobot_visio_update
				robot = self.bigrobot
			else:
				print("hello")
				event = self.event_minirobot_visio_update
				event = self.minirobot
			cds = eval(args[0])
			lingos = eval(args[1])
			objects = cds + lingos
			new_objects = Visio.relative_to_absolute(robot.pos, robot.a, objects)
			self.objects += new_objects
			event.set()
		else:
			self.send_error(canal, "Error %s.on_msg_visio (%s:%d) : pas assez de paramètres " %
				(self.__class__.__name__, currentframe().f_code.co_filename, currentframe().f_lineno))
	
	def send_error(self, canal, msg):
		if self.ircbot:
			self.ircbot.send_error(canal, msg)
		else:
			print("ERROR", msg)
	
	def robots(self):
		return (self.bigrobot, self.minirobot, self.enemy1, self.enemy2)

	def enemyrobots(self):
		return (self.enemy1, self.enemy2)

	def __repr__(self):
		return "GameState(%s)" % (self.robots(),)

	def print_stats(self):
		for k, s in self.sums.items():
			if s['n'] != 0:
				print(k, s['t']/s['n'])

