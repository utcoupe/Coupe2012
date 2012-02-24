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
from ia_utcoupe.ia import *

myia = IA("localhost", 6667,
	CANAL_BIG_ASSERV, CANAL_MINI_ASSERV, CANAL_HOKUYO, CANAL_DEBUG,
	(0,0), (0,0), (3000,0), (3000,0)
)

myia.start()







