


import threading



from clientIRC.iabot import *
from agents.robot import *

SUFFIX_MINI		= "mini"
CHAN_ASSERV		= "#asserv"

FILENAME_MAP	= "graph/map.xml"


ircbot = IABot("localhost", 6667, CHAN_ASSERV, CHAN_ASSERV+SUFFIX_MINI)
threading.Thread(None, ircbot.start, "loop iabot").start()

bigrobot = Robot(ircbot, CHAN_ASSERV)
minirobot = Robot(ircbot, CHAN_ASSERV+SUFFIX_MINI)

ng = NavGraph(230)
ng.load_xml(FILENAME_MAP)


from random import randrange
import time
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
