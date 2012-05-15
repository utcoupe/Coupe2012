#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
DIR_PATH = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join("..","..","..","lib"))

import threading

from py3irc.mypyirc import MyPyIrcBot

from radar import Radar


class RadarBot(MyPyIrcBot):
	def __init__(self, radar, serverip, serverport):
		super().__init__(serverip, serverport, "radarbot", ["#hokuyo"])
		self.canal = "#hokuyo"
		self.radar = radar

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
	bot = RadarBot(radar, "localhost", 6667)
	threading.Thread(target=bot.start).start()
	radar.mainloop()
