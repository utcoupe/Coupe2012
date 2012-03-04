#!/usr/bin/python3
# -*- coding: utf-8 -*-


import types

from lib.mypyirc.mypyircbot import MyPyIrcBot

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
		if self.serv:
			self.serv.disconnect("Tchuss")
				
	
