#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","lib"))

import threading
import subprocess
import time

from py3irc.mypyirc import bridgebot




class ExecutableBot(bridgebot.BridgeBot):
	def __init__(self, server_ip, server_port, nickname, channel, exec_name, exec_params, protocol_file, protocol_prefixe):
		super().__init__(server_ip, server_port, nickname, channel, protocol_file, protocol_prefixe)
		self.exec_name = exec_name
		self.exec_params = exec_params
		self.process = None
		self.lock_connect = threading.Lock()
		self.connect_exec()
		print("OK")
		

	def connect_exec(self):
		def f():
			while True:
				try:
					# kill de l'ancien process
					if self.process:
						try:
							self.process.terminate()
							time.sleep(1)
							if not self.process.poll():
								self.process.terminate()
						except Exception:
							self.process = None
					self.process = subprocess.Popen([self.exec_name]+self.exec_params, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
				except Exception as ex:
					print(ex)
					time.sleep(1)
				else:
					print("reconnection ok")
					self.sendall("reconnection ok")
					break
		if self.lock_connect.acquire(False):
			t = threading.Thread(target=f)
			t.setDaemon(True)
			t.start()
			self.lock_connect.release()
			
	
	def write(self, msg):
		""" #écrit sur l'input standart """
		if msg:
			print("%s" % msg.strip())
			msg = bytes(msg.strip()+"\n","utf-8")
			try:
				self.process.stdin.write(msg)
				self.process.stdin.flush()
			except IOError:
				print("coucou")
				self.connect_exec()
				return 'IOError'

	def read(self):
		try:
			self.process.stdout.flush()
			m = str(self.process.stdout.readline(),"utf-8")
		except IOError:
			print("hello")
			self.connect_exec()
			return 'IOError'
		else:
			return m

	def cmd__setexe(self, exe, args, *, id_msg=42, **kwargs):
		"""
		changer l'executable
		@param exe nom de l'executable
		@param args la liste des arguments, séparés par une virgule
		"""
		self.exec_name = exe
		self.exec_args = args[1:-1].split(',')
		self.connect_exec()

def run(**args):
	import optparse

	default = {}
	default["server_ip"] 			= "localhost"
	default["server_port"] 			= 6667
	default["nickname"]				= "pybot"
	default["channel"]				= "#test"
	default["exec_name"]			= "./test.py"
	default["exec_params"]			= ""
	default["protocol_file"]		= "testprotocol.h"
	default["protocol_prefixe"]	= "Q_"
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
		options.exec_name,
		options.exec_params,
		options.protocol_file,
		options.protocol_prefixe)
	bot = ExecutableBot(
		options.server_ip,
		options.server_port,
		options.nickname,
		options.channel,
		options.exec_name,
		options.exec_params.split(','),
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

	
	return bot

	
if __name__ == "__main__":
	run()
	
