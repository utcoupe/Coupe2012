#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os

sys.path.append(os.path.join("..","lib","py3irc"))

import irclib
import ircbot

class SimuIrcBot(ircbot.SingleServerIRCBot):
	def __init__(self, robot, server_ip, server_port):
		self.robot = robot
		self.serv = None
		self.nickname = "simubot"
		self.channel = "#asserv"
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

	def on_pubmsg(self, serv, ev):
		"""
		Méthode appelée à la réception d'un message, si le message est
		une commande, (<=> si il existe une fonction "cmd_{message}",
		alors on lance la fonction.
		@param serv le serveur
		@param ev
		"""
		self.serv = serv
		
		auteur = irclib.nm_to_n(ev.source())
		canal = ev.target()
		msg = ev.arguments()[0].strip().lower()
		msg_split = msg.split(" ")
		f_name = msg_split[0]
		params = msg_split[1:]

		if canal == "#asserv":
			if f_name == "goto":
				if len(params) == 3:
					self.robot.goto(*list(map(int, params)))
				else:
					self.send(canal, "pas le bon nombre d'arguments")


	def send(self, channel, msg):
		for m in msg.split("\n"):
			self.serv.privmsg(channel, m)

		
