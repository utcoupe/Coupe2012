#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","lib"))

import threading
import serial

from py3irc.mypyirc import BridgeBot




class ArduinoBot(BridgeBot):
	def __init__(self, server_ip, server_port, nickname, channel, serial_port, serial_baudrate, protocol_file, protocol_prefixe):
		self.serial = None
		try:
			self.serial = serial.Serial(serial_port, serial_baudrate, timeout=1, writeTimeout=1)
		except serial.SerialException as ex:
			print("Erreur lors de l'ouverture du port série",ex)
			sys.exit(1)
		print("OK")
		
		BridgeBot.__init__(self, server_ip, server_port, nickname, channel, protocol_file, protocol_prefixe)

	
	def write(self, msg):
		""" écrit sur le port série """
		print("%s" % msg.strip())
		msg = bytes(msg.strip()+"\n","utf-8")
		self.serial.write(msg)

	def read(self):
		return str(self.serial.readline(),"utf-8")

def run(**args):
	import optparse

	default = {}
	default["server_ip"] 		= "localhost"
	default["server_port"] 		= 6667
	default["nickname"]			= "pybot"
	default["channel"]			= "#test"
	default["serial_port"]		= "/dev/ttyACM0"
	default["serial_baudrate"]	= 115200
	default["protocol_file"]	= os.path.join(FILE_DIR,"..","protocol.h")
	default["protocol_prefixe"]= "Q_"
	default.update(args)
	
	usage = "usage: %prog [options]"
	parser = optparse.OptionParser(usage,version="%prog 0.0")
	parser.add_option("-S", "--server-ip",
						action="store", dest="server_ip", default=default["server_ip"],
						help="ip irc server")
	parser.add_option("-P", "--server-port",
						action="store", dest="server_port", type="int", default=default["server_port"],
						help="port irc server")
	parser.add_option("-n", "--nickname",
						action="store", dest="nickname", default=default["nickname"],
						help="nickname on irc")
	parser.add_option("-c", "--channel",
						action="store", dest="channel", default=default["channel"],
						help="channel on irc")
	parser.add_option("-s", "--serial-port",
						action="store", dest="serial_port", default=default["serial_port"],
						help="serial port")
	parser.add_option("-p", "--serial-baudrate",
						action="store", dest="serial_baudrate", default=default["serial_baudrate"],
						help="serial baudrate")
	parser.add_option("-f", "--protocol-file",
						action="store", dest="protocol_file", default=default["protocol_file"],
						help="protocol file")
	parser.add_option("-x", "--protocol-prefixe",
						action="store", dest="protocol_prefixe", default=default["protocol_prefixe"],
						help="protocol prefixe")
    
	(options, _args) = parser.parse_args()

	print(options.server_ip,
		options.server_port,
		options.nickname,
		options.channel,
		options.serial_port,
		options.serial_baudrate,
		options.protocol_file,
		options.protocol_prefixe)
	bot = ArduinoBot(
		options.server_ip,
		options.server_port,
		options.nickname,
		options.channel,
		options.serial_port,
		options.serial_baudrate,
		options.protocol_file,
		options.protocol_prefixe)
	#print(list(filter(lambda x: x[0:4] == 'cmd_', dir(bot))))

	if not args.get('blocking', True):
		t = threading.Thread(None, bot.start)
		t.setDaemon(True)
		t.start()
		return bot
	else:
		bot.start()

	
if __name__ == "__main__":
	run()
	
