#!/usr/bin/python3
# -*- coding: utf-8 -*-


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","lib"))

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


# création bot irc
ircbot = IABot("localhost", 6667, (CANAL_BIG_ASSERV, CANAL_MINI_ASSERV, CANAL_HOKYO, CANAL_DEBUG))

# création du module de débug
debug = Debug(ircbot, CANAL_DEBUG)

# création des deux robots
bigrobot = Robot(ircbot, CANAL_BIG_ASSERV)
minirobot = Robot(ircbot, CANAL_MINI_ASSERV)



# création du graph de déplacement
ng = NavGraph(R_ROBOT)
ng.load_xml(FILENAME_MAP)


# création du gamestate qui se rafraichi automatiquement
dpos = {}
dpos['big'] = (0,0)
dpos['mini'] = (0,0)
dpos['enemy1'] = (3000,0)
dpos['enemy2'] = (3000,0)
gamestate = GameState(ircbot, CANAL_BIG_ASSERV, CANAL_MINI_ASSERV, dpos)


# démarage du bot irc
t = threading.Thread(None, ircbot.start, "loop iabot")
t.setDaemon(True)
t.start()

# démarage du rafraichissement du gamestate
gamestate.start()


actions = get_actions_bigrobot(gamestate, bigrobot)

# mainloop
def mainloop():
	while 1:
		start = time.time()

		if not actions:
			print("PLUS D'ACTIONS A RÉALISER !")
		
		# si le robot n'est pas en action et qu'il reste des actions
		if not gamestate.bigrobot.in_action and actions:
			# recherche de la mailleur action à effectuer
			for action in actions:
				action.compute_score(gamestate.bigrobot.pos)
			best_action = min(actions, key=lambda a: a.score)

			print(best_action)
			# si cette action n'est pas déjà celle que le robot veut atteindre
			if gamestate.bigrobot.is_new_action(best_action):
				print("CHANGEMENT D'ACTION")
				# calcul du chemin
				final_pos = best_action.point_acces
				n,r,path = ng.get_path(gamestate.bigrobot.pos, final_pos)
				debug.draw_path(path, RED)
				print(gamestate.bigrobot.pos, final_pos, path)
				gamestate.bigrobot.set_target_action(best_action, path)

			# si le robot est arrivé
			if gamestate.bigrobot.is_arrive():
				print("YEEEES")
				actions.remove(best_action)
			else:
				# si le robot a atteind le prochain point de passage,
				# on passe au suivant
				if gamestate.bigrobot.reach_next_checkpoint():
					goal = gamestate.bigrobot.get_next_checkpoint()
					print("goto %s" % goal)
					bigrobot.cancel()
					if goal:
						bigrobot.goto(goal, 800)

		# debug
		debug.remove_circle(ID_DEBUG_ENEMY1)
		debug.draw_circle(gamestate.enemy1.pos, R_ENEMY, BLUE, ID_DEBUG_ENEMY1)
		
		debug.remove_circle(ID_DEBUG_ENEMY2)
		debug.draw_circle(gamestate.enemy2.pos, R_ENEMY, BLUE, ID_DEBUG_ENEMY2)
		
		debug.remove_circle(ID_DEBUG_BIGROBOT)
		debug.draw_circle(gamestate.bigrobot.pos, R_ENEMY, RED, ID_DEBUG_BIGROBOT)
		
		debug.remove_circle(ID_DEBUG_MINIROBOT)
		debug.draw_circle(gamestate.minirobot.pos, R_ENEMY, RED, ID_DEBUG_MINIROBOT)
		
		
		time_ellapsed = time.time() - start
		print(time_ellapsed)
		delay = max(0, 0.2 - time_ellapsed)
		time.sleep(delay)

input("appuyez sur une touche pour commencer\n")
mainloop()





