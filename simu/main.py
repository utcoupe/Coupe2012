#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from define import *

from simubot import *
from engine import *
from robot import *
from lingot import *
from wall import *
from cd import *

import threading


if __name__ == "__main__":
	robot = Robot()
	ircbot = SimuIrcBot(robot, "localhost", 6667)
	try:
		t = threading.Thread(None,ircbot.start,"simuircbot")
		t.start()
	except Exception as ex:
		print(ex)
	top_left = mm_to_px((0,0))
	top_right = mm_to_px((3000,0))
	down_left = mm_to_px((0,2000))
	down_right = mm_to_px((3000,2000))
	wall_top = Wall(top_left, top_right)
	wall_right = Wall(top_right, down_right)
	wall_down = Wall(down_left, down_right)
	wall_left = Wall(top_left, down_left)
	cd = Cd(mm_to_px((1500,1000)), "black")
	engine = Engine()
	engine.add(wall_top)
	engine.add(wall_down)
	engine.add(wall_left)
	engine.add(wall_right)
	for _ in range(50):
		engine.add(Cd(mm_to_px((1500,1000)), "white"))
	engine.add(robot)
	engine.add(cd)
	engine.start()
	
