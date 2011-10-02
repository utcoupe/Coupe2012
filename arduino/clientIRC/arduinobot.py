# -*- coding: utf-8 -*-

import sys
sys.path.append("../../lib/pyirclib")
import threading
import serial
import inspect

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
			sys.exit(1)
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
		cmd = "cmd_" + msg_split[0]
		if msg == "help":
			for f_name in dir(self):
				if "cmd_" in f_name:
					serv.privmsg(canal, f_name[4:] + ":" + getattr(self, f_name).__doc__.replace("\n"," ").replace("\r"," ").replace("\t"," "))
		elif hasattr(self, cmd):
			f = getattr(self, cmd)
			if len(msg_split) == len(inspect.getargspec(f).args):
				msg = bytes("0.0." + f(*msg_split[:-1])+"\n","utf-8")
				print (msg)
				self.serial.write(msg)
			else:
				serv.privmsg(canal, "invalid arg number")
	
	def loop(self):
		while True:
			msg = self.serial.readline()
			if msg and self.serv:
				self.serv.privmsg(self.channel, str(msg,"utf-8"))


if __name__ == "__main__":
	"""import sys
	if len(sys.argv) < 3:
		print "exe <serial-port> <bauderate>"
		sys.exit(1)
	
	port = sys.argv[1]
	try:
		baudrate = int(sys.argv[2])
	except ValueError:
		print "invalid bauderate"
		sys.exit(1)
	
	import serial

	s = serial.Serial(port, baudrate, timeout=1, writeTimeout=1)"""
	
	ArduinoBot("10.42.43.94",6667,"arduinobot","#test","/dev/ttyACM0",115200).start()
