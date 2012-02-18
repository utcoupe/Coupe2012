#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Simulateur permettant de simuler un match d'eurobot.

Déplacer gros robot: clic gauche
Déplacer petit robot: clic droit

Déplacer adversaire: idem + ctrl

IRC:
voir les canaux dans define.py
Chaque robot défini les canaux sur les quels il veut se connecter

@author Thomas Recouvreux
@author Pierre-Henry Fricot
@author Cédric Bache
"""
from define import *


from simubot import *
from engine import *
from bigrobot import *
from minirobot import *
from lingot import *
from wall import *
from cd import *
from totem import *
from tour import *
from match import *
from hokyo import *

import threading
from map import *

if __name__ == "__main__":
	match = Match()

	# robots
	bigrobot = BigRobot(CANAL_BIG_ASSERV, mm_to_px(250,250), BLUE)
	minirobot = MiniRobot(CANAL_MINI_ASSERV, mm_to_px(400,250), BLUE)
	bigrobot2 = BigRobot(CANAL_BIG_ASSERV+'2', mm_to_px(3000-250,250), RED)
	minirobot2 = MiniRobot(CANAL_MINI_ASSERV+'2', mm_to_px(3000-400,250), RED)
	robots = (bigrobot, minirobot, bigrobot2, minirobot2)

	# hokyo
	hokyo = Hokyo(CANAL_HOKYO, robots)

	# ircbot
	ircbot = SimuIrcBot("localhost", 6667, (CANAL_BIG_ASSERV,CANAL_MINI_ASSERV,CANAL_BIG_ASSERV+'2',CANAL_MINI_ASSERV+'2',CANAL_HOKYO))
	ircbot.add_executer(hokyo)
	for i,robot in enumerate(robots):
		ircbot.add_executer(robot)
	
	engine = Engine(ircbot.stop, match)
	try:
		t = threading.Thread(None,ircbot.start,"simuircbot")
		t.start()
	except Exception as ex:
		print(ex)
		
	loadMap("map.xml",engine)
	
	engine.add(bigrobot)
	engine.add(minirobot)
	engine.add(bigrobot2)
	engine.add(minirobot2)

	
	engine.start()
	
