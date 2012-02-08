#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os

sys.path.append(os.path.join("..","lib","py3irc"))

import irclib
import ircbot

import threading
import time

from define import *



class SimuIrcBot(ircbot.SingleServerIRCBot):
	def __init__(self, robots, server_ip, server_port):
		self.robots = robots
		self.serv = None
		self.nickname = "simubot"
		ircbot.SingleServerIRCBot.__init__(self,
			[(server_ip, server_port)],
			self.nickname,
			"Bot du simulateur",
			1
		)
		self.running = True
		self.t = threading.Thread(None, self.loop_send, "simubot-loopsend")
		self.t.start()
		

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
		self.serv = serv
		for r in self.robots:
			serv.join(r.canal_asserv)

	def on_pubmsg(self, serv, ev):
		"""
		Méthode appelée à la réception d'un message
		"""
		self.serv = serv
		
		auteur = irclib.nm_to_n(ev.source())
		canal = ev.target()
		msg = ev.arguments()[0].strip().lower()
		msg_split = msg.split(" ")
		f_name = msg_split[0]
		params = msg_split[1:]

		for r in self.robots:
			if canal == r.canal_asserv:
				self.onMsgFromAsserv(canal, r, f_name, params)

	def onMsgFromAsserv(self, canal, robot, f_name, params):
		if f_name == "id":
			send("asserv")
		elif f_name == "ping":
			send("pong")
		elif f_name == "cancel":
			robot.cmd_cancel()
		elif f_name == "goto":
			if len(params) == 3:
				robot.cmd_goto(*list(map(int, params)))
			else:
				self.send(canal, "pas le bon nombre d'arguments")
		elif f_name == "gotor":
			if len(params) == 3:
				robot.cmd_gotor(*list(map(int, params)))
			else:
				self.send(canal, "pas le bon nombre d'arguments")
		elif f_name == "turn":
			if len(params) == 3:
				robot.cmd_turn(*list(map(int, params)))
			else:
				self.send(canal, "pas le bon nombre d'arguments")
		elif f_name == "turnr":
			if len(params) == 3:
				robot.cmd_turnr(*list(map(int, params)))
			else:
				self.send(canal, "pas le bon nombre d'arguments")
		elif f_name == "pos":
			robot.cmd_pos()
		elif f_name == "acalib":
			if len(params) == 1:
				robot.cmd_acalib(*list(map(int, params)))
			else:
				self.send(canal, "pas le bon nombre d'arguments")
		elif f_name == "stop":
			robot.cmd_stop()
		elif f_name == "resume":
			robot.cmd_resume()
		

	
	def send(self, channel, msg):
		if self.serv:
			for m in str(msg).split("\n"):
				self.serv.privmsg(channel, m)

	def stop(self):
		self.running = False
		if self.serv:
			self.serv.disconnect("Tchuss")

	def loop_send(self):
		while self.running:
			for r in self.robots:
				canal, msg = r.get_msg()
				while canal:
					self.send(canal,msg)
					canal, msg = r.get_msg()
			time.sleep(0.01)
				
	
