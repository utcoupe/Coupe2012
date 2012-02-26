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

import optparse

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
from debug import *

import threading
from map import *

if __name__ == "__main__":
	
	default = {}
	default["server_ip"] 		= "localhost"
	default["server_port"] 		= 6667
	default.update(args)

	usage = "usage: %prog [options]"
	parser = optparse.OptionParser(usage,version="%prog 0.0")
	parser.add_option("-S", "--server-ip",
						action="store", dest="server_ip", default=default["server_ip"],
						help="ip irc server")
	parser.add_option("-P", "--server-port",
						action="store", dest="server_port", type="int", default=default["server_port"],
						help="port irc server")
	(options, args) = parser.parse_args()


	
	match = Match()

	# debug
	debug = Debug()

	# robots
	bigrobot = BigRobot(CANAL_BIG_ASSERV, mm_to_px(250,250), BLUE)
	minirobot = MiniRobot(CANAL_MINI_ASSERV, mm_to_px(400,250), BLUE)
	bigrobot2 = BigRobot(CANAL_BIG_ASSERV+'2', mm_to_px(3000-250,250), RED)
	minirobot2 = MiniRobot(CANAL_MINI_ASSERV+'2', mm_to_px(3000-400,250), RED)
	robots = (bigrobot, minirobot, bigrobot2, minirobot2)

	# hokuyo
	hokuyo = Hokuyo(CANAL_HOKUYO, robots)

	# ircbot
	ircbot = SimuIrcBot(options.server_ip, options.server_port, (CANAL_BIG_ASSERV,CANAL_MINI_ASSERV,CANAL_BIG_ASSERV+'2',CANAL_MINI_ASSERV+'2',CANAL_HOKUYO, CANAL_DEBUG))
	ircbot.add_executer(debug)
	ircbot.add_executer(hokuyo)
	for i,robot in enumerate(robots):
		ircbot.add_executer(robot)
	
	engine = Engine(ircbot.stop, match, debug)
	try:
		t = threading.Thread(None,ircbot.start,"simuircbot")
		t.setDaemon(True)
		t.start()
	except Exception as ex:
		print(ex)
		
	loadMap("map.xml",engine)
	
	engine.add(bigrobot)
	engine.add(minirobot)
	engine.add(bigrobot2)
	engine.add(minirobot2)

	engine.start()
	
