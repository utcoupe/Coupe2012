# -*- coding: utf-8 -*-

import threading
import time


import mypyircbot
from ircdefine import *


class BridgeBot(mypyircbot.MyPyIrcBot):
	def __init__(self, server_ip, server_port, nickname, channel, protocole_file, protocole_prefixe):
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

		self.thread = threading.Thread(None,self.loop,"executablebotloop")
		self.thread.start()



	def make_cmd_function(self, cmd_name, id_cmd, params, doc):
		"""
		Cré une fonction
		f(self, param1, param2, ... , **kwargs) -> "kwargs['id_msg']<SEP><id_cmd><SEP>param1<SEP>param2<SEP>..."
		Cette fonction peut être surchargée pour définir un protocole autre que celui
		par default.
		
		@param cmd_name nom de la commande dans le protocole
		@param id_cmd id de la commande dans le protocole
		@param 
		"""
		code = '''
def f(%s):
	return '%s'.join([str(kwargs["id_msg"]), '%s', %s])
		''' % (','.join(['self']+params+['**kwargs']), SEP, id_cmd, ','.join(params))
		d = {}
		exec(code, d)
		d['f'].__doc__ = doc.strip()
		return d['f']


	def write_rep(self, msg):
		""" Cette fonction doit être surchargée, elle doit envoyer un message à l'objet """
		raise Exception("%s.write_rep : cette méthode doit être surchargée" % self.__class__.__name__)



	def read(self):
		""" Cette fonction doit être surchargée, elle doit lire un message de l'objet """
		raise Exception("%s.read : cette méthode doit être surchargée" % self.__class__.__name__)


	def loop(self):
		""" boucle de lecture des I/O standarts du programme, le message est lu puis envoyé
		sur le serveur IRC """
		while True:
			try:
				msg = self.read()
			except Exception as ex:
				print(ex)
				time.sleep(2)
			else:
				if msg and self.serv:
					self.serv.privmsg(self.channel, msg)
					

