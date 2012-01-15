

from vec import *
from map import *
from pathfinding import *



import doctest
doctest.testmod()



if __name__ == "__main__":
	mymap = Map("map.xml")
	mymap.draw()
	#mymap.print_collision_matrix()
	mymap.mainloop()
