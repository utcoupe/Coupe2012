#!/usr/bin/env python3
# -*- coding: utf-8 -*-


from define import *


from simubot import *
from engine import *
from robot import *
from lingot import *
from wall import *
from cd import *
from totem import *
from tour import *

import threading


if __name__ == "__main__":
	robot = Robot(mm_to_px((250,250)))
	ircbot = SimuIrcBot(robot, "localhost", 6667)
	engine = Engine(ircbot.stop)
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
	mur_depart_gauche = Wall(mm_to_px((0,500)),mm_to_px((500,500)))
	mur_depart_droite = Wall(mm_to_px((2500,500)),mm_to_px((3000,500)))
	mur_bateau_gauche = Wall(mm_to_px((325,2000)),mm_to_px((360,1260))) 
	mur_bateau_droite = Wall(mm_to_px((2675,2000)),mm_to_px((2640,1260))) 
	totem_left = Totem(mm_to_px((1100,1000)))
	totem_right = Totem(mm_to_px((1900,1000)))
	cd = Cd(mm_to_px((1500,1000)), "black")
	tour = Tour();
	
	engine.add(mur_depart_gauche)
	engine.add(mur_depart_droite)
	engine.add(mur_bateau_gauche)
	engine.add(mur_bateau_droite)
	engine.add(wall_top)
	engine.add(wall_down)
	engine.add(wall_left)
	engine.add(wall_right)
	engine.add(robot)
	engine.add(cd)
	engine.add(totem_left)
	engine.add(totem_right)
	engine.add(tour);
	

	
	for _ in range(50):
		engine.add(Cd(mm_to_px((1500,1000)), "white"))
		
	
	engine.start()
	
