# -*- coding: utf-8 -*-
"""
	@author: Thomas Recouvreux
"""



import inspect
import re
import threading
import collections
import time
import types

from ..py3irc import ircbot,irclib

from .ircdefine import *


def channel_to_prefix_cmd(canal):
	if canal[0] == '#':
		canal = canal[1:]
	return "cmd_%s_" % canal

def irc_cmd_to_func_name(canal, irc_cmd):
	if canal[0] == '#':
		canal = canal[1:]
	return channel_to_prefix_cmd(canal) + irc_cmd.lower()

def func_name_to_irc_cmd(f_name):
	return f_name.split('_',2)[2]

def replace_channel_in_f_name(f_name, new_channel):
	irc_cmd = func_name_to_irc_cmd(f_name)
	return irc_cmd_to_func_name(new_channel, irc_cmd)

def raw_msg_to_msg_n_options(raw_msg):
	"""
	
	"""
	raw_msg = raw_msg.strip().lower()
	if '#' in raw_msg:
		msg, str_options = raw_msg.split('#',1)
		msg.strip()
		str_options.strip()
	else:
		msg, str_options = raw_msg, ""
		
	options = {
		'id_msg': '42'
	}
	specs = {
		'id_msg': "id=(?P<id_msg>[-0-9]+)"
	}
	if str_options:
		for i,spec in specs.items():
			t = re.search(spec, str_options)
			if t:
				options[i] = t.group(i)

	return msg, options


class Executer:
	"""
	Classe permettant de lier facilement un objet à des commandes irc,
	Il suffit de faire hériter un objet de cette classe, puis d'utiliser
	la fonction mypyircbot.add_executer pour que toutes les fonctions
	de type "cmd_canal_irccmd" soient liées au bot.

	{@code
	class MaClass(Executer):
		cmd_test_hello(self, **kwargs):
			self.send("#test", 'coucou')

	bot = MyPyIrcBot(...)
	a = MaClass()
	bot.add_executer(a)
	
	--------------------------------------
	
	irc #test
	me : hello
	bot : coucou
	}
	"""
	def __init__(self):
		# messages à envoyer
		self.to_send = collections.deque()
	
	def get_msg(self):
		if self.to_send:
			return self.to_send.popleft()
		else:
			return (None,None)

	def send(self, canal, *msg):
		self.to_send.append((canal,self.compute_msg(*msg)))
	
	def compute_msg(self, *args):
		return SEP.join(map(lambda x: str(x), args))

	def transform(self, old_canal, new_canal):
		"""
		Renomme toutes les fonctions "_cmd_<old_canal>_irc_cmd en "cmd_<new_canal>_irc_cmd.
		Note : les aciennes fonctions ne sont pas supprimées.
		"""
		# être sûr qu'on ne se trimbal pas le '#'
		if old_canal[0] == '#': old_canal = old_canal[1:]
		if new_canal[0] == '#': new_canal = new_canal[1:]
		# effectuer le changement
		for f_name in dir(self):
			if f_name.startswith('_'+channel_to_prefix_cmd(old_canal)):
				new_f_name = replace_channel_in_f_name(f_name[1:], new_canal)
				setattr(self, new_f_name, getattr(self, f_name))


class MyPyIrcBot(ircbot.SingleServerIRCBot):
	"""
	Classe de base pour tous les clients IRC de ce projet, elle permet de
	facilité l'ajout de commandes et la génération automatique d'un texte
	d'aide en réponse à la commande 'help'
	Il y a deux façons d'ajouter une commande, soit en créant une classe
	qui hérite de cette classe, soit en utilisant la méthode "add_executer"

	Hériter de cette classe:
	
	Il suffit d'ajouter des méthodes avec un nom de type "cmd_<canal>_<irccmd>"
	{@code
	class Bot(MyPyIrcBot):
		cmd_test_hello(self, **kwargs):
			''' cette commande renvoie coucou '''
			self.send("#test", "id=%s coucou" % kwargs['id_msg'])
	
	--------------------------------------
	
	IRC #test:
	me : help
	bot : <affiche les commandes possibles>
	bot : help hello
	bot : cette commande renvoie coucou
	me : hello
	bot : id=<id_par_default> coucou
	me : hello # id=56
	bot : id=56 coucou
	}

	Hériter de Executer: #see mypyircbot.Executer
	"""
	def __init__(self, server_ip, server_port, nickname, channels):
		self.serv = None
		self.nickname = nickname
		self.canaux = list(( chan if chan[0] == '#' else '#'+chan for chan in channels ))

		# objets contenants des commandes à executer
		self.executers = {}
		
		ircbot.SingleServerIRCBot.__init__(self,
			[(server_ip, server_port)],
			nickname,
			"Bot réalisé en Python avec ircbot",
			2
		)
		
		self.t = threading.Thread(None, self._loop_executers, "mypyircbot-loop_executers")
		self.t.setDaemon(True)
		self.t.start()

		self.e_welcome = threading.Event()
	
	def on_nicknameinuse(self, serv, e):
		"""
		Callback quand le nom est déjà utilisé
		@param serv
		@param e
		"""
		self.nickname += "_"
		serv.nick(self.nickname + "_")
	
	def on_welcome(self, serv, ev):
		"""
		Méthode appelée une fois connecté et identifié.
		Notez qu'on ne peut rejoindre les canaux auparavant.
		"""
		for chan in self.canaux:
			serv.join(chan)
		self.serv = serv
		self.e_welcome.set()

	def wait_connection(self, timeout=None):
		self.e_welcome.wait(timeout)
	
	def write_rep(self, msg, id_msg=42):
		"""
		Fonction qui redistribue le message retourné par les fonctions "cmd_*".
		@param msg le message à envoyer
		"""
		print("%s (id=%s)" %(msg,id_msg))
		
	def on_pubmsg(self, serv, ev):
		"""
		Méthode appelée à la réception d'un message, si le message est
		une commande, (<=> si il existe une fonction "cmd_{message}",
		alors on lance la fonction.
		@param serv le serveur
		@param ev
		"""
		self.serv = serv
		
		auteur = irclib.nm_to_n(ev.source())
		canal = ev.target().strip().lower()
		msg = ev.arguments()[0]

		self._on_pubmsg(auteur, canal, msg)

	def _on_pubmsg(self, auteur, canal, msg):
		msg, options = raw_msg_to_msg_n_options(masg)
		msg_split = msg.strip().split(" ")
		f_name = self.irc_cmd_to_func_name(canal, msg_split[0])
		if msg_split[0] == "help":
			if len(msg_split) > 1:
				self.print_doc(canal, self.irc_cmd_to_func_name(canal, msg_split[1]))
			else:
				for f_name in dir(self):
					if f_name.startswith(self.channel_to_prefix_cmd(canal)):
						self.print_doc(canal, f_name)
		elif hasattr(self, f_name):
			f = getattr(self, f_name)
			f_args = inspect.getargspec(f).args
			nb_args = len(f_args)
			args = msg_split[1:]
			if 'self' in f_args:
				nb_args -= 1
			if len(args) == nb_args:
				self.write_rep(f(*args,**options)+"\n")
			else:
				self.send(canal, "invalid arg number : need %s and get %s" % (str(inspect.getargspec(f)),msg_split))
		


	def get_protocole(self, str_protocole, prefix):
		"""
		Récupérer le protocole dans le fichier .h précisé.
		Les commandes doivent être formater de la sorte :
		{@code
		/**
		Documentation
		\@param abc
		\@param t
		*/
		#define {prefixe}NOM_DE_LA_COMMANDE		4
		}

		@param str_protocole
		@param prefix le prefixes des defines
		@return une liste de dictionnaires {id: ?, name: ?, params: ?, doc: ?}
		"""
		
		commands = []

		# spec des regexp
		spec_doc = '\/\*\*(?P<doc>(.(?!\*\/))*.)\*\/'
		spec_define = '#define\s+{prefix}(?P<name>\w+)\s+(?P<id>\d+)'.format(prefix=prefix)
		spec_cmd = spec_doc+"\s*"+spec_define
		spec_params = '@param\s+(?P<param>[a-zA-Z_]\w*)'

		# compilation de la regexp pour les paramètres
		re_params = re.compile(spec_params)

		# recherche des commandes
		for t in re.finditer(spec_cmd,str_protocole,re.DOTALL):
			params = list([p.group("param") for p in re_params.finditer(t.group('doc'))])
			commands.append({'id': int(t.group('id')), 'name': t.group('name'), 'params': params, 'doc': t.group('doc')})
			#print(commands[-1])

		return commands
		

	def get_protocole_multi_prefixes(self, str_protocole, prefixes):

		# initialisation du dictionnaire
		commands = {}
		for prefix in prefixes:
			commands[prefix] = []

		for prefix in prefixes:
			commands[prefix] = self.get_protocole(str_protocole, prefix)

		return commands
		
	def print_doc(self, canal, f_name):
		"""
		Afficher la doc d'une fonction,
		ex : print_doc(cmd_bidule, bidule) =>
		"bidule : voici la documentation de bidule"
		@param f_name le nom interne de la fonction
		@param cmd le nom à afficher
		"""
		try:
			doc = getattr(self, f_name).__doc__
		except AttributeError as ex:
			self.send(canal, str(ex))
		else:
			if not doc: doc = "No documentation"
			irc_cmd = self.func_name_to_irc_cmd(f_name)
			self.send(canal, " ")
			self.send(canal, "## "+irc_cmd.upper()+" ##")
			for line in doc.split("\n"):
				self.send(canal, line)
			self.send(canal, " ")

	def send(self, canal, msg):
		"""
		Envoie un message au serveur.
		@param msg le message à envoyer
		"""
		print("SEND", canal, msg)
		if self.serv:
			try:
				self.serv.privmsg(canal, msg)
			except irclib.ServerNotConnectedError as ex:
				print("send error", ex)

	def channel_to_prefix_cmd(self, canal):
		return channel_to_prefix_cmd(canal)
	
	def irc_cmd_to_func_name(self, canal, irc_cmd):
		return irc_cmd_to_func_name(canal, irc_cmd)

	def func_name_to_irc_cmd(self, f_name):
		return func_name_to_irc_cmd(f_name)

	def add_cmd_function(self, canal, irc_cmd, cmd_function):
		"""
		Ajouter une commande à la classe en cours.
		@param irc_cmd le nom de la commande
		@param cmd_function la lambda fonction
		"""
		setattr(self, self.irc_cmd_to_func_name(canal, irc_cmd), types.MethodType(cmd_function, self))

	def add_executer(self, executer):
		i = id(executer)
		self.executers[i] = executer
		for f_name in filter(lambda s: s.startswith("cmd_"), dir(executer)):
			f = getattr(executer, f_name)
			f_args = inspect.getargspec(f).args
			if len(f_args) > 0 and 'self' == f_args[0]:
				f_params = f_args+['**kwargs']
			else:
				f_params = ['self']+f_args+['**kwargs']
			if len(f_args) > 0 and 'self' == f_args[0]:
				robot_args = f_args[1:]
			else:
				robot_args == f_args
			robot_args = list(map(lambda s: 'int(%s)' %s, robot_args))
			robot_args.append('**kwargs')
			d = {}
			code = "def f(%s):\n" % ','.join(f_params)
			code += "	self.executers[{i}].{f_name}({args})\n".format(f_name=f_name, i=i, args=','.join(robot_args))
			code += "	return '%s'" % f_name
			exec(code, d)
			d['f'].__doc__ = f.__doc__
			f = types.MethodType(d['f'], self)
			setattr(self, f_name, f)

	def _loop_executers(self):
		while 1:
			for e in self.executers.values():
				canal, msg = e.get_msg()
				while canal:
					self.send(canal,msg)
					canal, msg = e.get_msg()
			time.sleep(0.01)




