import os
import sys
import threading
from subprocess import *
import time
from Tkinter import *

# En fonction de l'ecran 
sizeFact = 4.0

gbool=True
robot=[]
robotSize= 60.0
radbot=robotSize/sizeFact

separator='+'

p = Popen(["./hokuyoApp.exe","-col","1"], stdout=PIPE, stdin=PIPE)


class thRead(threading.Thread):
	def run(self):
		global robot
		while gbool:
			try:
				p.stdout.flush()
				r = p.stdout.readline()
				print "C++ message : ",r
			except Exception as ex:
				print ex
				exit()
	
			if(r.find(separator)!=-1):
				r = r.split(separator)
				robot = eval(r[1])
				print "Apres l'eval : ",robot
				drawZone(canvas)
					
			print "List affiche : ",robot
			print ""
			print ""

			
class thWrite(threading.Thread):
	def run(self):
		time.sleep(1)
		while gbool:
			start = time.time()
			cmd='1'+separator+'1\n'
			p.stdin.write(cmd.encode("utf-8"))
			p.stdin.flush()
			time.sleep(0.1 - (time.time()-start) )

def drawcircle(canv,x,y,rad):
    return canv.create_oval(x-rad,y-rad,x+rad,y+rad,width=0,fill='green')

def drawZone(canv):
	canv.delete(ALL)
	
	canv.create_line(0, 2000/(2*sizeFact), 3000, 2000/(2*sizeFact), fill='red')
	canv.create_line(3000/(2*sizeFact), 2000, 3000/(2*sizeFact), 0, fill='red')
	canv.create_line(3000/(4*sizeFact), 2000, 3000/(4*sizeFact), 0, fill='red')
	canv.create_line(3000*3/(4*sizeFact), 2000, 3000*3/(4*sizeFact), 0, fill='red')
	
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

cmd='1'+separator+'9\n'
p.stdin.write(cmd.encode("utf-8"))
p.stdin.flush()


