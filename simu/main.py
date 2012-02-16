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
	bigrobot = BigRobot("#asserv", mm_to_px(250,250), BLUE)
	minirobot = MiniRobot("#asservmini", mm_to_px(400,250), BLUE)
	bigrobot2 = BigRobot("#asserv2", mm_to_px(3000-250,250), RED)
	minirobot2 = MiniRobot("#asservmini2", mm_to_px(3000-400,250), RED)
	robots = (bigrobot, minirobot, bigrobot2, minirobot2)

	# hokyo
	hokyo = Hokyo("#hokyo", robots)

	# ircbot
	ircbot = SimuIrcBot("localhost", 6667, ("#asserv","#asservmini","#asserv2","#asservmini2","#hokyo"))
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
	
