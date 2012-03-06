import os
import sys
import threading
from subprocess import *
import time
from Tkinter import *

robotSize= 150
sizeFact = 5.0

gbool = True
robot=[]
radbot=robotSize/sizeFact

p = Popen(["./hokuyoApp"], stdout=PIPE, stderr=PIPE, stdin=PIPE)

class thRead(threading.Thread):
	def run(self):
		global robot
		while gbool:
			r = p.stdout.readline()
			p.stdout.flush()
			if(r.find('.')!=-1):
				del(robot[:])
				r = r.split('.')
				listCoor = eval(r[1])
				listCoor = listCoor[1:]
				for li in listCoor:				
					robot.append(li)
			
			
class thWrite(threading.Thread):
	def run(self):
		time.sleep(1)
		while gbool:
			cmd='1.1\n'			
			p.stdin.write(cmd.encode("utf-8"))
			p.stdin.flush()
			time.sleep(1)
			drawZone(canvas)


def drawcircle(canv,x,y,rad):
    return canv.create_oval(x-rad,y-rad,x+rad,y+rad,width=0,fill='green')

def drawZone(canv):
	canv.delete(ALL)
	
	for coor in robot:
		x=-1
		y=-1
		for c in coor:
			if x==-1:
				x=c/sizeFact
			else:
				y=c/sizeFact
		drawcircle(canv,x,y,radbot)








# ---
t1=thRead()
t2=thWrite()
t1.setDaemon(True)
t2.setDaemon(True)
t1.start()
t2.start()
# ---
root = Tk()

canvas = Canvas(width=3000/sizeFact, height=2000/sizeFact, bg='blue')  
canvas.pack(expand=YES, fill=BOTH) 

root.mainloop()


cmd='1.9\n'			
p.stdin.write(cmd.encode("utf-8"))
p.stdin.flush()


