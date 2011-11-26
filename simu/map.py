
import xml.dom.minidom

from lingot import *
from wall import *
from cd import *
from totem import *
from tour import *

def loadMap(filename,engine):
	
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
