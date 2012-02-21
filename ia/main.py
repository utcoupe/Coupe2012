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
from visualisation.graphview import *
from action import *

FILENAME_MAP	= "graph/map.xml"


# création bot irc
ircbot = IABot("localhost", 6667, (CANAL_BIG_ASSERV, CANAL_MINI_ASSERV, CANAL_HOKYO))

# création des deux robots
bigrobot = Robot(ircbot, CANAL_BIG_ASSERV)
minirobot = Robot(ircbot, CANAL_MINI_ASSERV)



# création du graph de déplacement
ng = NavGraph(200)
ng.load_xml(FILENAME_MAP)

# visualisation du navgraph
dynamic_obstacle = Poly().initFromCircle((4000,3000),200,8)
graphview = GraphView(ng,dynamic_obstacle)


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


def ramasser_totem():
	print("YOUHOU")
action_totem = Action((1100,1350), gamestate, ramasser_totem)
actions = [action_totem]

# mainloop
def mainloop():
	while 1:
		start = time.time()

		if not gamestate.bigrobot.in_action and actions:
			for action in actions:
				action.compute_score(gamestate.bigrobot.pos)
			best_action = min(actions, key=lambda a: a.score)
			if best_action.dist_from(gamestate.bigrobot.pos) <= 100:
				print("YEEEEES")
				gamestate.bigrobot.in_action = True
			else:
				print("COUCOU")
				final_pos = best_action.point_acces
				n,r,path = ng.get_path(gamestate.bigrobot.pos, final_pos)
				print(gamestate.bigrobot.pos, final_pos, path)
				if len(path) > 1:
					goal = path[1]
					if len(path) > 2 and (gamestate.bigrobot.pos - goal).norm2() <= 10000:
						goal = path[2]
					if not gamestate.bigrobot.current_goal or goal != gamestate.bigrobot.current_goal:
						print("goto %s" % goal)
						graphview.show_result_calc_path(n,r,path)
						gamestate.bigrobot.current_goal = goal
						bigrobot.cancel()
						bigrobot.goto(goal, 800)
				
		
		time_ellapsed = time.time() - start
		print(time_ellapsed)
		delay = max(0, 0.2 - time_ellapsed)
		time.sleep(delay)

t = threading.Thread(target=mainloop, name="ia-mainloop")
t.setDaemon(True)
input("appuyez sur une touche pour commencer")
t.start()


graphview.mainloop()



