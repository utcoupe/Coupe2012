# -*- coding: utf-8 -*-




if __name__ == "__main__":
	import sys
	import os
	FILE_DIR = os.path.dirname(os.path.abspath(__file__))
	sys.path.append(os.path.join(FILE_DIR,".."))
	sys.path.append(os.path.join(FILE_DIR,"..","..","lib"))
	
	import time
	
	from graphview import *
	from ia.graph.tile import TiledMap
	from geometry import Poly
	
	filename = os.path.join(FILE_DIR,"..","map.jpg")
	tm = TiledMap()
	start = time.time()
	tm.load_img(filename)
	print("img load time : %s" % (time.time() - start))
	
	
	dynamic_obstacle = Poly().initFromCircle((-500,-500),250,8)
	v = GraphView(tm,dynamic_obstacle)
	v.mainloop()			
