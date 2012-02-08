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



COMPILED_F = None
SEP = '.'


class ArduinoBot(mypyircbot.MyPyIrcBot):
	def __init__(self, server_ip, server_port, nickname, channel, serial_port, serial_baudrate, protocole_file=None, protocole_prefixe=None):
		"""
		Constructeur qui pourrait prendre des paramètres dans un "vrai" programme.
		"""
		mypyircbot.MyPyIrcBot.__init__(self, server_ip, server_port, nickname, channel)
		
		print("Récupération du protocole dans %s..." %protocole_file)
		self._get_protocole(protocole_file, protocole_prefixe)
		print("OK")
		print("Connection au port série %s..." % serial_port)
		try:
			self.serial = serial.Serial(serial_port, serial_baudrate, timeout=1, writeTimeout=1)
		except serial.SerialException as ex:
			print(ex)
			sys.exit(1)
		print("OK")
		
		self.thread = threading.Thread(None,self.loop,"arduinoloop")
		self.thread.start()
	
	def write_rep(self, msg):
		""" écrit sur le port série """
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

	def _get_protocole(self, f_name, prefixe):
		"""
		Récupérer le protocole dans le fichier .h précisé.
		Les commandes doivent être formater de la sorte :
		/**
		Documentation
		@param abc
		@param t
		*/
		#define {prefixe}NOM_DE_LA_COMMANDE		4
		il sera alors généré une fonction :
		def cmd_nom_de_la_commande(abc,t):
			return SEP.join(['4',abc,t])

		@param f_name le nom du fichier
		@param prefixe le prefixe des define
		"""
		f = open(f_name)
		
		spec_doc = '\/\*\*(?P<doc>(.(?!\*\/))*.)\*\/'
		spec_define = '#define\s+{prefixe}(?P<cmd>\w+)\s+(?P<id>\d+)'.format(prefixe=prefixe)
		spec_cmd = spec_doc+"\s"+spec_define

		spec_params = '@param\s+(?P<param>[a-zA-Z_]\w*)'
		re_params = re.compile(spec_params)
		for t in re.finditer(spec_cmd,f.read(),re.DOTALL):
			params = ','.join([p.group("param") for p in re_params.finditer(t.group('doc'))])
			self._add_cmd_function(t.group('cmd'), t.group('id'), params, t.group('doc'))
			print(t.group('id'), t.group('cmd')+'('+params+')')
		f.close()

	
	def _add_cmd_function(self, cmd_name, id_cmd, params, doc):
		"""
		Ajouter une commande à la classe en cours.
		@param cmd_name le nom de la commande
		@param id_cmd l'id de la commande dans le protocole
		"""
		global COMPILED_F
		exec("COMPILED_F = lambda {params}: '{SEP}'.join(['{id_cmd}',{params}])".format(params=params,SEP=SEP,id_cmd=str(id_cmd).lower()), globals())
		COMPILED_F.__doc__ = doc.strip()
		f_name = "cmd_"+cmd_name.lower()
		setattr(self, f_name, COMPILED_F)

def run(**args):
	import sys
	import optparse

	default = {}
	default["server_ip"] 		= "localhost"
	default["server_port"] 		= 6667
	default["nickname"]			= "pybot"
	default["channel"]			= "test"
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
		"#"+options.channel,
		options.serial_port,
		options.serial_baudrate,
		options.protocole_file,
		options.protocole_prefixe)
	#print(list(filter(lambda x: x[0:4] == 'cmd_', dir(bot))))
	bot.start()

	
if __name__ == "__main__":
	run()
	
