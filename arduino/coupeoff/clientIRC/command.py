#!/usr/bin/python3
# -*- coding:utf-8 -*-


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","..","lib"))

from py3irc.mypyirc.mypyircbot import MyPyIrcBot

import time
import threading
import tkinter as tk
import threading



class Bot(MyPyIrcBot):
	def __init__(self,serverip, serverport):
		MyPyIrcBot.__init__(self, serverip, serverport, "commandbot", ["#coupeoff"])
		self.canal = "#test"
	def send(self, msg):
		super().send("#coupeoff", msg)

bot = Bot('localhost',6667)
t = threading.Thread(target=bot.start)
t.setDaemon(True)
t.start()

class KeyEventBinding:
	def __init__(self, tkapp, keypress, keyrelease, keyhold):
		tkapp.bind_all('<KeyPress>', self.keypress)
		tkapp.bind_all('<KeyRelease>', self.keyrelease)

		self._keypress = keypress
		self._keyrelease = keyrelease
		self._keyhold = keyhold

		self.__t_press = 0
		self.__hold = False

	def keypress(self, event):
		self.__t_press = time.time()
		if not self.__hold:
			print("press", event.char)
			self.__press = True
			self._keypress(event)

	def keyrelease(self, event):
		def f():
			if time.time() - self.__t_press > 0.1:
				print("release", event.char)
				self.__hold = False
				self._keyrelease(event)
			else:
				self.__hold = True
				self._keyhold(event)
		threading.Timer(0.1, f).start()
		

states = {}
states['z'] = 0
states['q'] = 0
states['s'] = 0
states['d'] = 0

def send():
	bot.send(".pwm %s %s %s %s" % (states['z'], states['q'], states['s'], states['d']))


def keypress(event):
	global states
	if event.keysym == 'Escape':
		root.destroy()
	x = event.char
	if x=='z':
		states['z'] = 1
	elif x=='q':
		states['q'] = 1
	elif x=='d':
		states['d'] = 1
	elif x=='s':
		states['s'] = 1
	else:
		print(x)
	send()

def keyrelease(event):
	global states
	x = event.char
	if x=='z':
		states['z'] = 0
	elif x=='q':
		states['q'] = 0
	elif x=='d':
		states['d'] = 0
	elif x=='s':
		states['s'] = 0
	else:
		print(x)
	send()
	

root = tk.Tk()
print ("Press a key (Escape key to exit):")
#root.bind_all('<KeyPress>', keypress)
#root.bind_all('<KeyRelease>', keyrelease)
KeyEventBinding(root, keypress, keyrelease, lambda x: x)
# don't show the tk window
#root.withdraw()
root.mainloop()
