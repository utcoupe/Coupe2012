

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

FILENAME_MAP	= "graph/map.xml"


ircbot = IABot("localhost", 6667, (CANAL_BIG_ASSERV, CANAL_MINI_ASSERV, CANAL_HOKYO))

bigrobot = Robot(ircbot, CANAL_BIG_ASSERV)
minirobot = Robot(ircbot, CANAL_MINI_ASSERV)

ng = NavGraph(230)
ng.load_xml(FILENAME_MAP)

dpos = {}
dpos['big'] = (0,0)
dpos['mini'] = (0,0)
dpos['enemy1'] = (3000,0)
dpos['enemy2'] = (3000,0)
gamestate = GameState(ircbot, CANAL_BIG_ASSERV, CANAL_MINI_ASSERV)

threading.Thread(None, ircbot.start, "loop iabot").start()
gamestate.start()

while 1:
	print(gamestate)
	time.sleep(1)

from random import randrange
while True:
	bigrobot.cancel()
	bigrobot.update_pos()
	time.sleep(0.1)
	path = []
	while not path:
		x = randrange(0,3000)
		y = randrange(0,2000)
		_,_,path = ng.get_path(bigrobot.pos, (x,y))
	print(path)
	for p in path:
		bigrobot.goto(p.round(),5000)
	time.sleep(3)
