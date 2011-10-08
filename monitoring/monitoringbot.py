#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","lib","py3irc"))


import myircbot

import subprocess

BASH = os.path.join(FILE_DIR,"bat.sh")
print(os.path.join(FILE_DIR,"bat.sh"))

class MonitoringBot(myircbot.MyIRCBot):
	def __init__(self, server_ip, server_port):
		myircbot.MyIRCBot.__init__(self, server_ip, server_port, "monitoringbot", "#monitoring")

	def cmd_batterie(self,f):
		""" [format] Affiche l'état de la batterie (p:%,t:time) """
		p = subprocess.Popen([BASH,f], stdout=subprocess.PIPE)
		msg = str(p.communicate()[0], "utf-8").strip()
		return msg if msg else "No informations"


if __name__ == "__main__":
	bot = MonitoringBot("10.42.43.94", 6667)
	bot.start()
