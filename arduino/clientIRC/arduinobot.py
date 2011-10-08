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

import myircbot



COMPILED_F = None
SEP = '.'


class ArduinoBot(myircbot.MyIRCBot):
	def __init__(self, server_ip, server_port, nickname, channel, serial_port, serial_baudrate, protocole_file=None, protocole_prefixe=None):
		"""
		Constructeur qui pourrait prendre des paramètres dans un "vrai" programme.
		"""
		myircbot.MyIRCBot.__init__(self, server_ip, server_port, nickname, channel)
		
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
		msg = bytes(f(*msg_split[1:])+"\n","utf-8")
		self.serial.write(msg)
		
	
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

	def _get_protocole(self, f_name, prefixe):
		f = open(f_name)
		re_begin = '(#define)?\s+{prefixe}(?P<var>\w+)\s+'.format(prefixe=prefixe)
		re_end = '(\s)*\/\/\s*(?P<doc>\[(?P<params>(\w|,|\s)*)].*)'
		re_char = re.compile(re_begin+'[\'"](?P<value>[^\s|\/]+)[\'"]'+re_end)
		re_int = re.compile(re_begin+'(?P<value>[^\s|\/]+)'+re_end)
		for line in f:
			# recherche char
			t = re_char.match(line)
			if t:
				self._add_cmd_function(t.group('var'), t.group('value'), t.group('params'), t.group('doc'))
				#print(t.group('var'),"\t= (str)",t.group('value'),"\tdoc= ",t.group('doc'), t.group('doc'))
			else:
				# recherche int
				t = re_int.match(line)
				if t:
					try:
						value = int(t.group('value'))
					except:
						value = None
					self._add_cmd_function(t.group('var'), t.group('value'), t.group('params'), t.group('doc'))
					#print(t.group('var'),"\t= (int)",value,"\tparams= ",t.group('params'),"\tdoc= ",t.group('doc'))
		f.close()

	"""
	Ajouter une commande à la classe en cours.
	@param cmd_name le nom de la commande
	@param id_cmd l'id de la commande dans le protocole
	"""
	def _add_cmd_function(self, cmd_name, id_cmd, params, doc):
		global COMPILED_F
		exec("COMPILED_F = lambda {params}: '{SEP}'.join(['0','0','{id_cmd}',{params}])".format(params=params,SEP=SEP,id_cmd=str(id_cmd).lower()), globals())
		COMPILED_F.__doc__ = doc.strip()
		f_name = "cmd_"+cmd_name.lower()
		setattr(self, f_name, COMPILED_F)

def run(**args):
	import sys
	import optparse
	
	server_ip = args["server_ip"] if "server_ip" in args else "10.42.43.94"
	server_port = args["server_port"] if "server_port" in args else 6667
	nickname = args["nickname"] if "nickname" in args else "testbot"
	channel = args["channel"] if "channel" in args else "test"
	serial_port = args["serial_port"] if "serial_port" in args else "/dev/ttyACM0"
	serial_baudrate = args["serial_baudrate"] if "serial_baudrate" in args else 115200
	protocole_file = args["protocole_file"] if "protocole_file" in args else "protocole.h"
	protocole_prefixe = args["protocole_prefixe"] if "protocole_prefixe" in args else "QA_"

	
	usage = "usage: %prog [options]"
	parser = optparse.OptionParser(usage,version="%prog 0.0")
	parser.add_option("-S", "--server-ip",
						action="store", dest="server_ip", default=server_ip,
						help="ip irc server")
	parser.add_option("-P", "--server-port",
						action="store", dest="server_port", type="int", default=server_port,
						help="port irc server")
	parser.add_option("-n", "--nickname",
						action="store", dest="nickname", default=nickname,
						help="nickname on irc")
	parser.add_option("-c", "--channel",
						action="store", dest="channel", default=channel,
						help="channel on irc")
	parser.add_option("-s", "--serial-port",
						action="store", dest="serial_port", default=serial_port,
						help="serial port")
	parser.add_option("-p", "--serial-baudrate",
						action="store", dest="serial_baudrate", default=serial_baudrate,
						help="serial baudrate")
	parser.add_option("-f", "--protocole-file",
						action="store", dest="protocole_file", default=protocole_file,
						help="protocole file")
	parser.add_option("-x", "--protocole-prefixe",
						action="store", dest="protocole_prefixe", default=protocole_prefixe,
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
	
