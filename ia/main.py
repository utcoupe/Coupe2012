#!/usr/bin/python3
# -*- coding: utf-8 -*-


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","lib"))

import optparse

from py3irc.mypyirc.ircdefine import *
from ia import *



default = {}
default["server_ip"] 		= "localhost"
default["server_port"] 		= 6667
default["ia"]				= 'u'
default["player_team"]		='blue'

usage = "usage: %prog [options]"
parser = optparse.OptionParser(usage,version="%prog 0.0")
parser.add_option("-S", "--server-ip",
					action="store", dest="server_ip", default=default["server_ip"],
					help="ip irc server")
parser.add_option("-P", "--server-port",
					action="store", dest="server_port", type="int", default=default["server_port"],
					help="port irc server")
parser.add_option("-i", "--ia",
					action="store", dest="ia", default=default["ia"],
					help="ia à utiliser, u->utcoupe, b->basic, d->dummy")
parser.add_option("-p", "--player-team",
					action="store", dest="player_team", default=default["player_team"],
					help="équipe du bot, red ou blue")
(options, args) = parser.parse_args()

if 		'b' == options.ia:
	IaChoosen = IaBase
elif 	'd' == options.ia:
	IaChoosen = IaDummy
elif 	'u' == options.ia:
	IaChoosen = IaUtcoupe
else:
	IaChoosen = IaUtcoupe


t = (options.player_team != 'blue')

	
myia = IaChoosen(
	options.server_ip, options.server_port,
	(0,0), (0,0), (3000,0), (3000,0),
	canal_big_asserv	= CANAL_BIG_ASSERV + ("2" if t else ""),
	canal_mini_asserv	= CANAL_MINI_ASSERV + ("2" if t else ""),
	canal_big_others	= CANAL_BIG_OTHERS + ("2" if t else ""),
	canal_mini_others	= CANAL_MINI_OTHERS + ("2" if t else ""),
	canal_debug			= CANAL_DEBUG + ("2" if t else ""),
	canal_hokuyo		= CANAL_HOKUYO,
)


try:
	myia.start()
except KeyboardInterrupt as ex:
	myia.stop()








