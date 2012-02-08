


from clientIRC.iabot import *
from agents.robot import *
import threading

SUFFIX_MINI		= "mini"
CHAN_ASSERV		= "#asserv"



ircbot = IABot("localhost", 6667, CHAN_ASSERV, CHAN_ASSERV+SUFFIX_MINI)
threading.Thread(None, ircbot.start, "loop iabot").start()
bigrobot = Robot(ircbot, CHAN_ASSERV)
minirobot = Robot(ircbot, CHAN_ASSERV+SUFFIX_MINI)

from random import randrange
import time
while True:
	x = randrange(0,3000)
	y = randrange(0,2000)
	bigrobot.cancel()
	bigrobot.goto((x,y),3000)
	time.sleep(1)
	x = randrange(0,3000)
	y = randrange(0,2000)
	minirobot.cancel()
	minirobot.goto((x,y),3000)
	time.sleep(1)

