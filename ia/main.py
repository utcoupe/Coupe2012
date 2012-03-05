#!/usr/bin/python3
# -*- coding: utf-8 -*-


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","lib"))

import optparse

from py3irc.mypyirc.ircdefine import *
from ia import IaUtcoupe



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

myia = IaUtcoupe(options.server_ip, options.server_port,
	CANAL_BIG_ASSERV, CANAL_MINI_ASSERV, CANAL_HOKUYO, CANAL_DEBUG,
	(0,0), (0,0), (3000,0), (3000,0)
)

try:
	myia.start()
except KeyboardInterrupt as ex:
	myia.stop()








