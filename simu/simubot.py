#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os

sys.path.append(os.path.join("..","lib","py3irc"))

import irclib
import ircbot

class SimuIrcBot(ircbot.SingleServerIRCBot):
	def __init__(self, server_ip, server_port):
		self.serv = None
		self.nickname = "simubot"
		self.channel = "test"
		ircbot.SingleServerIRCBot.__init__(self,
			[(server_ip, server_port)],
			self.nickname,
			"Bot du simulateur",
			1
		)

	def on_nicknameinuse(self, serv, e):
		"""
		Callback quand le nom est déjà utilisé
		@param serv
		@param e
		"""
		self.nickname += "_"
		serv.nick(self.nickname + "_")
	
	def on_welcome(self, serv, ev):
		"""
		Méthode appelée une fois connecté et identifié.
		Notez qu'on ne peut rejoindre les canaux auparavant.
		"""
		serv.join(self.channel)
		self.serv = serv

	
