import os
import sys
import threading
from subprocess import *
import time
from Tkinter import *

robotSize= 100
sizeFact = 4.0

gbool = True
robot=[]
radbot=robotSize/sizeFact

separator='+'

p = Popen(["./hokuyoApp.exe","-col","2"], stdout=PIPE, stdin=PIPE)

class thRead(threading.Thread):
	def run(self):
		global robot
		while gbool:
			try:
				p.stdout.flush()
				r = p.stdout.readline()
				print r
			except Exception as ex:
				print ex
				exit()
	
			if(r.find(separator)!=-1):
				del(robot[:])
			
				r = r.split(separator)

				listCoor = eval(r[1])
				listCoor = listCoor[1:]
				for li in listCoor:				
					robot.append(li)
			
			
class thWrite(threading.Thread):
	def run(self):
		time.sleep(1)
		while gbool:
			start = time.time()
			cmd='1.1\n'			
			p.stdin.write(cmd.encode("utf-8"))
			p.stdin.flush()
			time.sleep(0.1 - (time.time()-start) )
			drawZone(canvas)


def drawcircle(canv,x,y,rad):
    return canv.create_oval(x-rad,y-rad,x+rad,y+rad,width=0,fill='green')

def drawZone(canv):
	canv.delete(ALL)
	
	try: 
		for c in robot:
			x=c[0]/sizeFact
			y=(2000/sizeFact)-(c[1]/sizeFact)
			drawcircle(canv,x,y,radbot)
	except Exception as ex:
		print ex
		






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


