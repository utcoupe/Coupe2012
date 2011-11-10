#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Simulateur permettant de simuler un match d'eurobot.

Déplacer gros robot: clic gauche
Déplacer petit robot: clic droit

Déplacer adversaire: idem + ctrl

@author Thomas Recouvreux
@author Pierre-Henry Fricot
@author Cédric Bache
"""

from define import *


from simubot import *
from engine import *
from bigrobot import *
from minirobot import *
from lingot import *
from wall import *
from cd import *
from totem import *
from tour import *
import xml.dom.minidom


def load_map(filename,engine):
	
	ofi = open(filename, 'r')
	document = ofi.read()
	ofi.close()
	
	dom = xml.dom.minidom.parseString(document)
	for cd in dom.getElementsByTagName("cd"):
		engine.add(Cd(mm_to_px(int(cd.getAttribute("x")),int(cd.getAttribute("y"))),cd.getAttribute("color")))
		
	for wall in dom.getElementsByTagName("wall"):
		engine.add(Wall(mm_to_px(int(wall.getAttribute("x1")),int(wall.getAttribute("y1"))),mm_to_px(int(wall.getAttribute("x2")),int(wall.getAttribute("y2")))))
	
	for totem in dom.getElementsByTagName("totem"):
		engine.add(Totem(mm_to_px(int(totem.getAttribute("x")),int(totem.getAttribute("y")))))
	
	for tour in dom.getElementsByTagName("tour"):
		engine.add(Tour())

import threading

if __name__ == "__main__":
	bigrobot = BigRobot(mm_to_px(250,250), "blue", 1)
	minirobot = MiniRobot(mm_to_px(400,250), "blue", 1)
	bigrobot2 = BigRobot(mm_to_px(3000-250,250), "red", 2)
	minirobot2 = MiniRobot(mm_to_px(3000-400,250), "red", 2)
	ircbot = SimuIrcBot((bigrobot, minirobot, bigrobot2, minirobot2), "127.0.0.1", 6667)
	engine = Engine(ircbot.stop)
	try:
		t = threading.Thread(None,ircbot.start,"simuircbot")
		t.start()
	except Exception as ex:
		print(ex)
		
	load_map("map.xml",engine)
	
	engine.add(bigrobot)
	engine.add(minirobot)
	engine.add(bigrobot2)
	engine.add(minirobot2)
		
	
	engine.start()
	
