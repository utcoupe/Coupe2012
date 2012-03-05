#!/usr/bin/python3
# -*- coding: utf-8 -*-


import types

from py3irc.mypyirc.mypyircbot import MyPyIrcBot

from .define import *




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
		MyPyIrcBot.stop(self)
				
	
