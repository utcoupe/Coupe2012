import os
import sys
import threading
from subprocess import *
import time
from Tkinter import *
import math

MAX_VAL  = 4000
sizeFact = 10.0
Center = (MAX_VAL,MAX_VAL) # x,y

gbool = True
listC=[]
p = Popen(["./radDriver.exe"], stdout=PIPE, stdin=PIPE)

class thRead(threading.Thread):
	def run(self):
		global listC
		while gbool:
			
			try:
				p.stdout.flush()
				r = p.stdout.readline()
				
				#r="[(-0.79,1500),(0.79,1500),(-0.3,2000),(0.3,2000),(1.57,1000)]"
				listC = eval(r)
				print listC
				print "Data in"				

			except Exception as ex:
				print ex
			
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
	
	#drawcircle(canv,MAX_VAL/sizeFact,MAX_VAL/sizeFact,20)
	
	canv.create_line((Center[0]/sizeFact,Center[1]/sizeFact,Center[0]/sizeFact,(Center[1]-3000)/sizeFact),fill="red",width=2)
	canv.create_line((Center[0]/sizeFact,(Center[1]-3000)/sizeFact,(Center[0]-2000)/sizeFact,(Center[1]-3000)/sizeFact),fill="red",width=2)
	canv.create_line((Center[0]/sizeFact,(Center[1]-3000)/sizeFact,(Center[0]+2000)/sizeFact,(Center[1]-3000)/sizeFact),fill="red",width=2)
	canv.create_line(((Center[0]-2000)/sizeFact,Center[1]/sizeFact,(Center[0]-2000)/sizeFact,(Center[1]-3000)/sizeFact),fill="red",width=2)
	canv.create_line(((Center[0]+2000)/sizeFact,Center[1]/sizeFact,(Center[0]+2000)/sizeFact,(Center[1]-3000)/sizeFact),fill="red",width=2)
	
	
	for c in listC : 
		teta=float(c[0])
		lon =float(c[1])
	
		# conversion
		y = lon * math.cos(teta)
		x = lon * math.sin(teta)
		
		# decalage du centre
		x = MAX_VAL - x
		y = MAX_VAL - y 
		
		
		print  teta, " - ", lon, " - ", x, " - ", y
		
		x = x / sizeFact
		y = y / sizeFact
		
		x = int(x)
		y = int(y)
	
		
	
		drawcircle(canv,x,y,5)
	
	








# ---
t1=thRead()
t2=thWrite()
t1.setDaemon(True)
t2.setDaemon(True)
t1.start()
t2.start()
# ---
root = Tk()

canvas = Canvas(width=(2*MAX_VAL)/sizeFact, height=(MAX_VAL)/sizeFact, bg='blue')  
canvas.pack(expand=YES, fill=BOTH) 

root.mainloop()


cmd='1.9\n'			
p.stdin.write(cmd.encode("utf-8"))
p.stdin.flush()
