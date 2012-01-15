

from vec import *
from map import *
from pathfinding import *



import doctest
doctest.testmod()



if __name__ == "__main__":
	racine=tk.Tk()
	canvas=tk.Canvas(racine, width=600, height=400, background='white')
	canvas.pack()
	mymap = Map("map.xml",canvas)
	mymap.draw()
	#mymap.print_collision_matrix()
	racine.mainloop()
