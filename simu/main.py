# -*- coding: utf-8 -*-


from engine import *
from robot import *
from lingot import *
from cd import *



if __name__ == "__main__":
	robot = Robot()
	engine = Engine()
	engine.add(robot)
	engine.start()
