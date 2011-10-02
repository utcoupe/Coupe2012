# -*- coding: utf-8 -*-

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","lib","py3irc"))

import threading
import serial
import inspect
import time


import irclib
import ircbot

class ArduinoBot(ircbot.SingleServerIRCBot):
	def __init__(self, server_ip, server_port, nickname, channel, serial_port, serial_baudrate):
		"""
		Constructeur qui pourrait prendre des paramètres dans un "vrai" programme.
		"""
		ircbot.SingleServerIRCBot.__init__(self, [(server_ip, server_port)],
										   nickname, "Bot réalisé en Python avec ircbot")

		try:
			self.serial = serial.Serial(serial_port, serial_baudrate, timeout=1, writeTimeout=1)
		except serial.SerialException as ex:
			print(ex)
			#sys.exit(1)
		self.serv = None
		self.nickname = nickname
		self.channel = channel
		self.thread = threading.Thread(None,self.loop,"arduinoloop")
		self.thread.start()
	
	def on_nicknameinuse(self, serv, e):
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
		Méthode appelée à la réception d'un message, qui exclut son expéditeur s'il
		écrit une insulte.
		"""
		self.serv = serv
		
		auteur = irclib.nm_to_n(ev.source())
		canal = ev.target()
		msg = ev.arguments()[0].lower()
		msg_split = msg.split(" ")
		f_name = "cmd_" + msg_split[0]
		if msg_split[0] == "help":
			if len(msg_split) > 1:
				self.print_doc("cmd_"+msg_split[1], msg_split[1])
			else:
				for f_name in dir(self):
					if "cmd_" in f_name:
						self.print_doc(f_name, f_name[4:])
		elif hasattr(self, f_name):
			f = getattr(self, f_name)
			if len(msg_split) == len(inspect.getargspec(f).args):
				msg = bytes("0.0." + f(*msg_split[1:])+"\n","utf-8")
				print (msg)
				self.serial.write(msg)
			else:
				serv.privmsg(canal, "invalid arg number")
	
	def loop(self):
		while True:
			try:
				msg = self.serial.readline()
			except Exception as ex:
				print(ex)
				time.sleep(2)
			else:
				if msg and self.serv:
					self.serv.privmsg(self.channel, str(msg,"utf-8"))

	def print_doc(self, f_name, cmd=None):
		cmd = f_name if not cmd else cmd
		try:
			doc = getattr(self, f_name).__doc__
		except AttributeError as ex:
			self.serv.privmsg(self.channel, str(ex))
		else:
			for line in doc.split("\n"):
				self.serv.privmsg(self.channel, cmd + ":" + line)


if __name__ == "__main__":
	ArduinoBot("10.42.43.94",6667,"arduinobot","#test","/dev/ttyACM0",115200).start()
