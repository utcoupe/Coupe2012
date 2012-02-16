#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
sys.path.append(os.path.join("..","lib"))

import types

from py3irc.mypyircbot import *


from define import *




class SimuIrcBot(MyPyIrcBot):
	def __init__(self, server_ip, server_port, channels):
		MyPyIrcBot.__init__(self,
			server_ip, server_port,
			"simubot",
			channels
		)

		self.running = True


	def stop(self):
		self.running = False
		if self.serv:
			self.serv.disconnect("Tchuss")

				
	
