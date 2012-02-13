#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","lib","py3irc"))

import threading
import serial
import time
import re

import mypyircbot
from mypyircbot import *




class ArduinoBot(mypyircbot.MyPyIrcBot):
	def __init__(self, server_ip, server_port, nickname, channel, serial_port, serial_baudrate, protocole_file=None, protocole_prefixe=None):
		"""
		Constructeur qui pourrait prendre des paramètres dans un "vrai" programme.
		"""
		mypyircbot.MyPyIrcBot.__init__(self, server_ip, server_port, nickname, [channel])

		self.channel = channel

		f = open(protocole_file)
		str_protocole = f.read()
		f.close()
		
		print("Récupération du protocole dans %s..." %protocole_file)
		for cmd in self.get_protocole(str_protocole, protocole_prefixe):
			f_cmd = self.make_cmd_function(cmd['name'], cmd['id'], cmd['params'], cmd['doc'])
			self.add_cmd_function(channel, cmd['name'], f_cmd)
		print("OK")
		print("Connection au port série %s..." % serial_port)
		try:
			self.serial = serial.Serial(serial_port, serial_baudrate, timeout=1, writeTimeout=1)
		except serial.SerialException as ex:
			print(ex)
			#sys.exit(1)
		print("OK")
		
		self.thread = threading.Thread(None,self.loop,"arduinoloop")
		self.thread.start()
	
	def write_rep(self, msg):
		""" écrit sur le port série """
		print("%s" % msg.strip())
		msg = bytes(msg.strip()+"\n","utf-8")
		self.serial.write(msg)
	
	def loop(self):
		""" boucle de lecture du port série, le message est lu puis envoyé
		sur le serveur IRC """
		while True:
			try:
				msg = self.serial.readline()
			except Exception as ex:
				print(ex)
				time.sleep(2)
			else:
				if msg and self.serv:
					self.serv.privmsg(self.channel, str(msg,"utf-8"))
					
	def make_cmd_function(self, cmd_name, id_cmd, params, doc):
		"""
		@param cmd_name nom de la commande dans le protocole
		@param id_cmd id de la commande dans le protocole
		@param 
		"""
		global _COMPILED_F_
		code = '''
def f(%s):
	return '%s'.join([str(kwargs["id_msg"]), '%s', %s])
		''' % (','.join(params+['**kwargs']), SEP, id_cmd, ','.join(params))
		d = {}
		exec(code, d)
		d['f'].__doc__ = doc.strip()
		return d['f']

def run(**args):
	import optparse

	default = {}
	default["server_ip"] 		= "localhost"
	default["server_port"] 		= 6667
	default["nickname"]			= "pybot"
	default["channel"]			= "#test"
	default["serial_port"]		= "/dev/ttyACM0"
	default["serial_baudrate"]	= 115200
	default["protocole_file"]	= "protocole.h"
	default["protocole_prefixe"]= "QA_"
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
	parser.add_option("-f", "--protocole-file",
						action="store", dest="protocole_file", default=default["protocole_file"],
						help="protocole file")
	parser.add_option("-x", "--protocole-prefixe",
						action="store", dest="protocole_prefixe", default=default["protocole_prefixe"],
						help="protocole prefixe")
    
	(options, args) = parser.parse_args()

	print(options.server_ip,
		options.server_port,
		options.nickname,
		options.channel,
		options.serial_port,
		options.serial_baudrate,
		options.protocole_file,
		options.protocole_prefixe)
	bot = ArduinoBot(
		options.server_ip,
		options.server_port,
		options.nickname,
		options.channel,
		options.serial_port,
		options.serial_baudrate,
		options.protocole_file,
		options.protocole_prefixe)
	#print(list(filter(lambda x: x[0:4] == 'cmd_', dir(bot))))
	bot.start()
	return bot

	
if __name__ == "__main__":
	run()
	
