import os
import sys
import threading
from subprocess import *
import time
import math
from Tkinter import *

X=600.0
Y=400.0

pi=3.14

radbot = 5

def drawcircle(canv,x,y,rad):
    return canv.create_oval(x-rad,y-rad,x+rad,y+rad,width=0,fill='green')

def drawZone(canv):
	
	global X
	global Y
	
	canv.delete(ALL)
	
	canv.create_line(0, Y, X, Y)
	canv.create_line(X, 0, X, Y)
	
	#Grande diagonale
	born = 33.69
	
	for teta in range(0,90):
		if teta <= born:
			l=600/math.cos(math.radians(teta))
			x=l*math.cos(math.radians(teta))
			y=l*math.sin(math.radians(teta))
			drawcircle(canv,x,y,radbot)
		else:
			l=400/math.cos(math.radians(90-teta))
			x=l*math.cos(math.radians(teta))
			y=l*math.sin(math.radians(teta))
			drawcircle(canv,x,y,radbot)

# ---
root = Tk()

canvas = Canvas(width=800, height=600, bg='blue')  
canvas.pack(expand=YES, fill=BOTH) 

drawZone(canvas)

root.mainloop()





