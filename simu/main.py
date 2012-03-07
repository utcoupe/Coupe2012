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


import sys
import os
DIR_PATH = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(DIR_PATH, ".."))



import optparse
import threading
import time

from simu import *
from py3irc.mypyirc.ircdefine import *


if __name__ == "__main__":
	
	default = {}
	default["server_ip"] 		= "localhost"
	default["server_port"] 		= 6667

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
	bigrobot = BigRobot(
		canal_asserv		= CANAL_BIG_ASSERV,
		canal_others		= CANAL_BIG_OTHERS,
		posinit				= mm_to_px(250,250),
		team				= BLUE
	)
	minirobot = MiniRobot(
		canal_asserv		= CANAL_MINI_ASSERV,
		canal_others		= CANAL_MINI_OTHERS,
		posinit				= mm_to_px(400,250),
		team				= BLUE,
		match				= match
	)
	bigrobot2 = BigRobot(
		canal_asserv		= CANAL_BIG_ASSERV+'2',
		canal_others		= CANAL_BIG_OTHERS+'2',
		posinit				= mm_to_px(3000-250,250),
		team				= RED
	)
	minirobot2 = MiniRobot(
		canal_asserv		= CANAL_MINI_ASSERV+'2',
		canal_others		= CANAL_MINI_OTHERS+'2',
		posinit				= mm_to_px(3000-400,250),
		team				= RED,
		match				= match
	)
	robots = (bigrobot, minirobot, bigrobot2, minirobot2)

	# hokuyo
	hokuyo = Hokuyo(CANAL_HOKUYO, robots)

	# ircbot
	canaux = (
		CANAL_BIG_ASSERV,CANAL_MINI_ASSERV,CANAL_BIG_ASSERV+'2',CANAL_MINI_ASSERV+'2',
		CANAL_BIG_OTHERS,CANAL_MINI_OTHERS,CANAL_BIG_OTHERS+'2',CANAL_MINI_OTHERS+'2',
		CANAL_HOKUYO, CANAL_DEBUG
	)
	ircbot = SimuIrcBot(options.server_ip, options.server_port, canaux)
	ircbot.add_executer(debug)
	ircbot.add_executer(hokuyo)
	for i,robot in enumerate(robots):
		ircbot.add_executer(robot)
	
	engine = Engine(debug)

	engine.init(ircbot.stop,match)
	match.init(engine)
	bigrobot.init(engine)
	minirobot.init(engine)
	bigrobot2.init(engine)
	minirobot2.init(engine)
	try:
		t = threading.Thread(None,ircbot.start,"simuircbot")
		t.setDaemon(True)
		t.start()
	except Exception as ex:
		print(ex)
		
	load_map("map.xml",engine)
	
	engine.add(bigrobot)
	engine.add(minirobot)
	engine.add(bigrobot2)
	engine.add(minirobot2)

	t=threading.Thread(target=engine.start)
	t.setDaemon(True)
	t.start()

	while not engine.e_stop.is_set():
		try:
			engine.e_stop.wait(3)
			print(match.score(BLUE))
			print(match.score(RED))
		except KeyboardInterrupt:
			engine.stop()
			break
	
