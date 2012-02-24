import os
import sys
import threading
from subprocess import *
import time
from Tkinter import *

gbool = True
robot=[]
radbot=40

p = Popen(["./hokuyoApp"], stdout=PIPE, stderr=PIPE, stdin=PIPE)

class thRead(threading.Thread):
	def run(self):
		while gbool:
			r = p.stdout.readline()
			p.stdout.flush()
			print r
			if(r.find('.')!=-1):
				listCoor = eval(r)
				listCoor = listCoor[1:]
				for li in listCoor:				
					robot.append(li)
			
			
class thWrite(threading.Thread):
	def run(self):
		while gbool:
			print 'CMD'
			cmd='1.1'			
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
				x=c
			else:
				y=c
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

canvas = Canvas(width=600, height=400, bg='blue')  
canvas.pack(expand=YES, fill=BOTH) 

root.mainloop()





