# -*- coding: utf-8 -*-



# -*- coding: utf8 -*-

import irclib
import ircbot
import thread
import serial

class ArduinoBot(ircbot.SingleServerIRCBot):
	def __init__(self, server_ip, server_port, nickname, channel, serial_port, serial_baudrate):
		"""
		Constructeur qui pourrait prendre des paramètres dans un "vrai" programme.
		"""
		ircbot.SingleServerIRCBot.__init__(self, [(server_ip, server_port)],
										   nickname, "Bot réalisé en Python avec ircbot")

		self.serial = serial.Serial(port, baudrate, timeout=1, writeTimeout=1)
		self.serv = None
		self.nickname = nickname
		self.channel = channel
		thread.start_new_thread(self.loop,())
	
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
		message = ev.arguments()[0].lower()

		serv.privmsg(canal, "send : %s" % message)
		self.serial.write(ev.arguments()[0].strip().lower() + "\n")

	def loop(self):
		while True:
			msg = self.serial.readline()
			if msg and self.serv:
				self.serv.privmsg("#test", msg)


if __name__ == "__main__":
	import sys
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

	s = serial.Serial(port, baudrate, timeout=1, writeTimeout=1)
	
	BotModeration(s).start()
