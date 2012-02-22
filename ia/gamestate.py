


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","lib"))


import re
import time
import threading
from inspect import currentframe
import itertools

from geometry.vec import *
from mypyirc.ircdefine import *


# petite optimisation pour le calcul de k parmis 4, seul les valeurs de
# l'interval [0;4] sont calculées
C_K_PARMIS_4 = list(( tuple(itertools.permutations(range(4), i)) for i in range(5) ))
def permutation_k_parmis_4(k):
	return C_K_PARMIS_4[k]


class RobotState:
	def __init__(self, p):
		self.pos = Vec(p)
		self.a = 0
		self.time_pos_updated = 0
		self.in_action = False
		self.path = []
		self.target_action = None

	def reset_target_action(self):
		self.path = []
		self.target_action = None
	
	def set_new_path(self, path):
		self.path = path
		self.current_goal = path[0]

	def set_target_action(self, action, path):
		self.reset_target_action()
		self.target_action = action
		self.set_new_path(path)

	def reach_next_checkpoint(self):
		return (self.pos - self.path[0]).norm2() <= 100

	def get_next_checkpoint(self):
		self.path.pop(0)
		if self.path:
			return self.path[0]
		else:
			return None

	def is_arrive(self):
		return len(self.path) == 0

	def is_new_action(self, action):
		return action != self.target_action

	def update_pos(self, p):
		self.pos = Vec(p)
		self.time_pos_updated = time.time()

	def __repr__(self):
		return "RobotState(%s, %s, %s)" % (self.pos, self.a, self.time_pos_updated)

class StopableLoop:
	def __init__(self, func, sleep):
		self.func = func
		self.sleep = sleep
		self.event = threading.Event()
		self.event.clear()

	def start(self):
		if self.event.is_set():
			print("la boucle est déjà lancée !")
		else:
			while not self.event.is_set():
				self.func()
				self.event.wait(self.sleep)

	def stop(self):
		self.event.set()
		

class GameState:
	def __init__(self, ircbot, canal_big_asserv, canal_mini_asserv, dpos):
		self.ircbot 			= ircbot
		self.canal_big_asserv 	= canal_big_asserv
		self.canal_mini_asserv 	= canal_mini_asserv
		self.bigrobot 			= RobotState(dpos['big'])
		self.minirobot 			= RobotState(dpos['mini'])
		self.enemy1		 		= RobotState(dpos['enemy1'])
		self.enemy2			 	= RobotState(dpos['enemy2'])
		
		self.loops 				= []
		self.loops.append(StopableLoop(self._loop_ask_asserv_for_pos, 0.1))
		self.loops.append(StopableLoop(self._loop_ask_hokyo_for_pos, 0.1))

		if ircbot: # pour les doctest ircbot sera à None
			self.ircbot.add_listener(self.on_msg)

	def start(self):
		for loop in self.loops:
			t = threading.Thread(None, loop.start)
			t.setDaemon(True)
			t.start()

	def ask_hokyo_for_pos(self):
		self.ircbot.send(CANAL_HOKYO, "get # id=%s" % ID_MSG_HOKYO)

	def ask_asserv_for_pos(self, canal):
		self.ircbot.send(canal, "pos # id=%s" % ID_MSG_POS)
	
	def _loop_ask_asserv_for_pos(self):
		self.ask_asserv_for_pos(self.canal_big_asserv)
		self.ask_asserv_for_pos(self.canal_mini_asserv)

	def _loop_ask_hokyo_for_pos(self):
		self.ask_hokyo_for_pos()

	def on_msg(self, canal, auteur, msg):
		msg_split = msg.split(SEP)
		if len(msg_split) > 2:
			id_msg = int(msg_split[0])
			params = msg_split[1:]
			if ID_MSG_HOKYO == id_msg:
				self.on_msg_hokyo(params)
			elif ID_MSG_POS == id_msg:
				self.on_msg_pos(canal,params)

	def on_msg_pos(self, canal, params):
		if len(params) >= 3:
			# transformation des strings en int
			params = tuple(map(int, params))
			# choix du robot à update
			if canal == self.canal_big_asserv:
				robot_to_update = self.bigrobot
			elif canal == self.canal_mini_asserv:
				robot_to_update = self.minirobot
			else:
				print("Error %s.on_msg_pos (%s:%d) : canal non connu : %s " % (self.__class__.__name__, currentframe().f_code.co_filename, currentframe().f_lineno, canal))
			# update
			robot_to_update.update_pos(params[0:2])
			robot_to_update.a = params[2]
		else:
			print("Error %s.on_msg_pos (%s:%d) : pas assez de paramètres " % (self.__class__.__name__, currentframe().f_code.co_filename, currentframe().f_lineno, canal))
	

	def on_msg_hokyo(self, params):
		if len(params) >= 1:
			lpos = tuple(( eval(s) for s in params ))
			robots = self.robots()
			def test_permut(permut):
				l = ( (robots[j].pos - lpos[i]).norm2() for i,j in enumerate(permut) )
				return sum(l)
			best_permut = min(permutation_k_parmis_4(len(lpos)), key=lambda permut: test_permut(permut))
			for i,j in enumerate(best_permut):
				if robots[j] in self.enemyrobots():
					robots[j].update_pos(lpos[i])
		else:
			print("Error %s.on_msg_hokyo (%s:%d) : pas assez de paramètres " % (self.__class__.__name__, currentframe().f_code.co_filename, currentframe().f_lineno, canal))

	def robots(self):
		return (self.bigrobot, self.minirobot, self.enemy1, self.enemy2)

	def enemyrobots(self):
		return (self.enemy1, self.enemy2)

	def __repr__(self):
		return "GameState(%s)" % (self.robots(),)


import doctest
doctest.testfile("doctest/gamestate.txt")
