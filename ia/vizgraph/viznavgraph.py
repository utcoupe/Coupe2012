# -*- coding: utf-8 -*-



if __name__ == "__main__":
	import sys
	import os
	FILE_DIR = os.path.dirname(os.path.abspath(__file__))
	sys.path.append(os.path.join(FILE_DIR,".."))
	sys.path.append(os.path.join(FILE_DIR,"..","..","lib"))
	
	import time
	
	from graphview import *
	from ia.graph.nav import NavGraph
	from geometry import Poly
	
	filename = os.path.join(FILE_DIR,"..","map.xml")
	offset= sys.argv[1]
	start = time.time()
	ng = NavGraph(offset, filename)
	print("init time : %s" % (time.time() - start))
	

	dynamic_obstacle = Poly().initFromCircle((1500,1500),200,8)
	ng.add_dynamic_obstacle(dynamic_obstacle)
	ng.update()
	
	v = GraphView(ng,dynamic_obstacle)
	v.mainloop()

