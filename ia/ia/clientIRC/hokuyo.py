
from .basiclinker import BasicLinker
import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","lib"))
from py3irc.mypyirc.ircdefine import *


class Hokuyo(BasicLinker):
	def __init__(self, ircbot, channel):
		super().__init__(ircbot, channel)
		
	def get(self, **kwargs):
		return self.send("getdata", **kwargs)

	def setcolor(self, color, **kwargs):
		if color==RED:
			return self.send("SET_REDCOLOR")
		else:
			return self.send("SET_PURPLECOLOR")


