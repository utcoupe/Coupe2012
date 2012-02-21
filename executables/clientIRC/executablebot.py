#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","lib"))

import threading
import subprocess

from mypyirc import bridgebot




class ExecutableBot(bridgebot.BridgeBot):
	def __init__(self, server_ip, server_port, nickname, channel, exec_name, exec_params, protocole_file, protocole_prefixe):
		self.process = None
		try:
			self.process = subprocess.Popen([exec_name]+exec_params, stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
		except Exception as ex:
			print(ex)
			#sys.exit(1)
		print("OK")
		
		bridgebot.BridgeBot.__init__(self, server_ip, server_port, nickname, channel, protocole_file, protocole_prefixe)

	
	def write_rep(self, msg):
		""" écrit sur l'input standart """
		print("%s" % msg.strip())
		msg = bytes(msg.strip()+"\n","utf-8")
		self.process.stdin.write(msg)
		self.process.stdin.flush()

	def read(self):
		self.process.stdout.flush()
		return str(self.process.stdout.readline(),"utf-8")

def run(**args):
	import optparse

	default = {}
	default["server_ip"] 			= "localhost"
	default["server_port"] 			= 6667
	default["nickname"]				= "pybot"
	default["channel"]				= "#test"
	default["exec_name"]			= "./a.out"
	default["exec_params"]			= ""
	default["protocole_file"]		= "testprotocole.h"
	default["protocole_prefixe"]	= "Q_"
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
	parser.add_option("-e", "--exec-name",
						action="store", dest="exec_name", default=default["exec_name"],
						help="nom de l'executable")
	parser.add_option("-p", "--exec-params",
						action="store", dest="exec_params", default=default["exec_params"],
						help="paramètres de l'executable séparés par ','")
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
		options.exec_name,
		options.exec_params,
		options.protocole_file,
		options.protocole_prefixe)
	bot = ExecutableBot(
		options.server_ip,
		options.server_port,
		options.nickname,
		options.channel,
		options.exec_name,
		options.exec_params.split(','),
		options.protocole_file,
		options.protocole_prefixe)
	#print(list(filter(lambda x: x[0:4] == 'cmd_', dir(bot))))
	
	t = threading.Thread(None, bot.start)
	t.daemon = True
	t.start()
	
	return bot

	
if __name__ == "__main__":
	run()
	
