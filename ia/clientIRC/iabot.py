# -*- coding: utf-8 -*-


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","lib","py3irc"))


import threading
import time

import ircbot
import irclib


class IABot(ircbot.SingleServerIRCBot):
	def __init__(self, server_ip, server_port, channels):
		self.nickname = "iabot"
		ircbot.SingleServerIRCBot.__init__(self,
			[(server_ip, server_port)],
			self.nickname,
			"Bot de l'IA",
			1
		)
		self.serv = None
		self.chans = channels
		self.listeners = []
		
		self.event_on_connect = threading.Event()

	def stop(self):
		if self.serv:
			self.serv.disconnect("Tchuss")
	
	def on_nicknameinuse(self, serv, e):
		self.nickname += "_"
		serv.nick(self.nickname + "_")
	
	def on_welcome(self, serv, ev):
		self.serv = serv
		for chan in self.chans:
			serv.join(chan)
		time.sleep(0.5)
		print("CONNECTION SERVEUR IRC OK")
		self.event_on_connect.set()

	def on_pubmsg(self, serv, ev):
		"""
		Méthode appelée à la réception d'un message
		"""
		self.serv = serv
		
		auteur = irclib.nm_to_n(ev.source())
		canal = ev.target()
		msg = ev.arguments()[0].strip().lower()
		for listener in self.listeners:
			listener(canal, auteur, msg)

	def send(self, chan, msg):
		if self.serv:
			for m in str(msg).split("\n"):
				self.serv.privmsg(chan, m)

	def add_listener(self, listener):
		self.listeners.append(listener)


if __name__ == "__main__":
	from random import randrange
	import time
	bot = IABot("localhost", 6667, "#asserv", "#asservmini")
	def bidon():
		while True:
			x = randrange(0,3000)
			y = randrange(0,2000)
			bot.send_msg("#asserv", "cancel")
			bot.send_msg("#asservmini", "goto %s %s 2000" % (x,y))
			time.sleep(3)
	t = threading.Thread(None, bidon, "send")
	t.setDaemon(True)
	t.start()
	bot.start()



	

