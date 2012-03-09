# -*- coding: utf-8 -*-

import threading
import time
import re



from .mypyircbot import MyPyIrcBot
from .ircdefine import *

class ProtocolException(Exception): pass

class BridgeBot(MyPyIrcBot):
	def __init__(self, server_ip, server_port, nickname, channel, protocol_file, protocol_prefixe):
		MyPyIrcBot.__init__(self, server_ip, server_port, nickname, [channel])

		self.channel = channel

		if protocol_file:
			f = open(protocol_file)
			str_protocol = f.read()
			f.close()
			print("Récupération du protocol dans %s..." %protocol_file)
			for cmd in self.get_protocol(str_protocol, protocol_prefixe):
				f_cmd = self.make_cmd_function(cmd['name'], cmd['id'], cmd['params'], cmd['doc'])
				self.add_cmd_function(channel, cmd['name'], f_cmd)
			print("OK")
		else:
			print("warning, pas de fichier de protocol précisé, il faudra charger un protocol manuellement")


		self.thread = threading.Thread(None,self.loop,"executablebotloop")
		self.thread.setDaemon(True)

	def start(self):
		self.thread.start()
		MyPyIrcBot.start(self)


	def make_cmd_function(self, cmd_name, id_cmd, params, doc):
		"""
		Cré une fonction
		f(self, param1, param2, ... , **kwargs) -> "kwargs['id_msg']<SEP><id_cmd><SEP>param1<SEP>param2<SEP>..."
		Cette fonction peut être surchargée pour définir un protocol autre que celui
		par default.
		
		@param cmd_name nom de la commande dans le protocol
		@param id_cmd id de la commande dans le protocol
		@param 
		"""
		code = "def f(%s):" % ','.join(['self']+params+['**kwargs'])
		code += "    return '%s'.join([str(kwargs['id_msg']), '%s', %s])" % (self.sep, id_cmd, ','.join(params))
		d = {}
		exec(code, d)
		d['f'].__doc__ = doc.strip()
		return d['f']


	def write(self, msg):
		""" Cette fonction doit être surchargée, elle doit envoyer un message à l'objet """
		raise Exception("%s.write : cette méthode doit être surchargée" % self.__class__.__name__)


	def read(self):
		""" Cette fonction doit être surchargée, elle doit lire un message de l'objet """
		raise Exception("%s.read : cette méthode doit être surchargée" % self.__class__.__name__)

	def convert_obj_to_irc(self, msg):
		""" converti un message venant de l'objet en un message irc et son id """
		msg_split = msg.split(self.sep,1)
		id_msg = msg_split[0]
		return id_msg, msg_split.replace(self.sep, SEP)
		
	
	def loop(self):
		""" boucle de lecture des I/O standarts du programme, le message est lu puis envoyé
		sur le serveur IRC """
		while True:
			try:
				msg = self.read()
			except Exception as ex:
				send_error(self.channel, ex)
				time.sleep(2)
			else:
				if msg:
					i,msg = convert_obj_to_irc(msg)
					self.send_response(self.channel, i, msg)
	
	def get_protocol(self, str_protocol, prefix):
		"""
		Récupérer le protocol dans le fichier .h précisé.
		Les commandes doivent être formater de la sorte :
		{@code
		/**
		Documentation
		\@param abc
		\@param t
		*/
		#define {prefixe}NOM_DE_LA_COMMANDE		4
		}

		@param str_protocol
		@param prefix le prefixes des defines
		@return une liste de dictionnaires {id: ?, name: ?, params: ?, doc: ?} + le caracère de séparation
		"""
		
		commands = []
		sep = '+'

		# spec des regexp
		spec_sep = '#define\s+SEP\s+[\'"](?P<sep>.)[\'"]'
		spec_doc = '\/\*\*(?P<doc>(.(?!\*\/))*.)\*\/'
		spec_define = '#define\s+{prefix}(?P<name>\w+)\s+(?P<id>\d+)'.format(prefix=prefix)
		spec_cmd = spec_doc+"\s*"+spec_define
		spec_params = '@param\s+(?P<param>[a-zA-Z_]\w*)'

		# compilation de la regexp des params car elle es appellée plusieurs fois
		re_params = re.compile(spec_params)

		# recherche du caractère de séparation
		t = re.search(spec_sep, str_protocol)
		if t:
			sep = t.group("sep")
		else:
			raise ProtocolException("le protocol de contient pas de caractère de séparation")

		# recherche des commandes
		for t in re.finditer(spec_cmd,str_protocol,re.DOTALL):
			params = list([p.group("param") for p in re_params.finditer(t.group('doc'))])
			commands.append({'id': int(t.group('id')), 'name': t.group('name'), 'params': params, 'doc': t.group('doc')})
			#print(commands[-1])

		return sep,commands
	
