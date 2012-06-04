#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
DIR_PATH = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join("..","..","..","lib"))

import threading
import time

from py3irc.mypyirc import MyPyIrcBot
from py3irc.mypyirc.ircdefine import *

from radar import Radar


class RadarBot(MyPyIrcBot):
	def __init__(self, radar, serverip, serverport, autoloop):
		super().__init__(serverip, serverport, "radarbot", [CANAL_HOKUYO])
		self.canal = CANAL_HOKUYO
		self.radar = radar
		if autoloop:
			print("coucou")
			t = threading.Thread(target=self.loop)
			t.setDaemon(True)
			t.start()
			

	def loop(self):
		while 1:
			print("heyho")
			self.send(CANAL_HOKUYO, ".getdata")
			time.sleep(0.5)

	def cmd__getdata(self, **kwargs):
		"""
			Commande définie juste pour éviter de générer un message
			d'erreur sur irc lorsque un getdata est effectué
		"""
		pass

	def cmd__response(self, data, **kwargs):
		"""
			@param exploite les données renvoyées par l'hokuyo
		"""
		try:
			points = eval(data)
		except:
			pass
		else:
			self.radar.draw_points(points)


if __name__ == "__main__":
	radar = Radar()
	bot = RadarBot(radar, "10.42.43.1", 6667, True)
	t = threading.Thread(target=bot.start)
	t.setDaemon(True)
	t.start()
	radar.mainloop()
