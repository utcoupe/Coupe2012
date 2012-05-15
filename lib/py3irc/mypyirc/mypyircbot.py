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
import traceback

from ..py3irc import ircbot,irclib

from .ircdefine import *
from .ircutils import *


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
			
	def send_color(self, canal, msg, *, color=None, background=None, bold=False):
		self.send(canal, add_color(msg, color=color, background=background, bold=bold))
		
	def send_error(self, canal, *msg):
		msg=self.compute_msg(*msg)
		self._send_color(canal, msg, color='white', background="red", bold=True)
		self._send_color(CANAL_ERRORS, msg, background="red", bold=True)

	def send_response(self, canal, id_msg, msg):
		self.send(canal, SEP.join((PREFIX_CMD+"response",str(msg).strip()))+SEP+"id_msg="+str(id_msg))

	def send(self, canal, *msg):
		self._send(canal,self.compute_msg(*msg))
	
	def _send(self, canal, msg):
		self.to_send.append((canal,msg))
		
	
	def compute_msg(self, *args):
		return SEP.join(map(lambda x: str(x), args))

	def transform(self, old_canal, new_canal):
		"""
		Renomme toutes les fonctions "_cmd_<old_canal>_irc_cmd en "cmd_<new_canal>_irc_cmd.
		Note : les aciennes fonctions ne sont pas supprimées.
		"""
		# être sûr qu'on ne se trimbal pas le '#'
		old_canal = canal_clientnormalize(old_canal)
		new_canal = canal_clientnormalize(new_canal)
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
		def cmd_test_hello(self, **kwargs):
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
		self.canaux = list( map(canal_ircnormalize, [CANAL_ERRORS]+list(channels)) )

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
		print("Serveur %s joined" % serv.get_server_name())
		for chan in self.canaux:
			serv.join(chan)
			print("joined %s" % chan)
		self.serv = serv
		self.e_welcome.set()

	def wait_connection(self, timeout=None):
		self.e_welcome.wait(timeout)
		
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
		if msg.startswith(PREFIX_CMD):
			args, kwargs = raw_msg_to_args_n_kwargs(msg[1:])
			self._on_cmd(canal, args[0], *args[1:], **kwargs)
			
	def _on_cmd(self, canal, irc_cmd, *args, **options):
		f = None
		# une fonction spécifique à ce channel
		f_name = irc_cmd_to_func_name(canal, irc_cmd)
		# une fonction commune à tous les channels
		f_name_glob = irc_cmd_to_func_name_global(irc_cmd)
		if hasattr(self, f_name):
			f = getattr(self, f_name)
		elif hasattr(self, f_name_glob):
			f = getattr(self, f_name_glob)
		if f:
			options["canal"] = canal
			argspec = inspect.getfullargspec(f)
			f_args = argspec.args
			try:
				self.write(f(*args,**options))
			except TypeError as ex:
				self.send_error(canal, "%s, need : %s, get = %s" % (ex,argspec,(args,options)))
		else:
			self.send_error(canal, "Invalid function name : %s" % irc_cmd)




	def cmd__help(self, irc_cmd=None, *, canal, id_msg=42):
		"""
		Afficher les fonctions de ce canal
		"""
		# affichage de l'help d'une fonction seulement
		if irc_cmd:
			f = None
			# une fonction spécifique à ce channel
			f_name = irc_cmd_to_func_name(canal, irc_cmd)
			# une fonction commune à tous les channels
			f_name_glob = irc_cmd_to_func_name_global(irc_cmd)
			if hasattr(self, f_name):
				f = f_name
			elif hasattr(self, f_name_glob):
				f = f_name_glob
			if f:
				self.print_doc(canal, f)
			else:
				self.send_error(canal, "Invalid function name : %s" % irc_cmd)
		# affichage de l'help de toutes les fonctions
		else:
			for f_name in dir(self):
				if f_name.startswith(channel_to_prefix_cmd(canal)) or f_name.startswith(prefix_cmd_global()):
					self.print_doc(canal, f_name)
	
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
			self.send_error(canal, str(ex))
		else:
			if not doc: doc = "No documentation"
			irc_cmd = func_name_to_irc_cmd(f_name)
			self.send(canal, " ")
			self.send(canal, "## "+irc_cmd.upper()+" ##")
			for line in doc.split("\n"):
				self.send(canal, line)
			self.send(canal, " ")

	def write(self, msg):
		if msg: print(msg)

	def sendall(self, msg):
		for canal in self.canaux:
			self.send(canal, msg)
	
	def send(self, canal, msg):
		"""
		Envoie un message au serveur.
		@param msg le message à envoyer
		"""
		#print("SEND", canal, msg)
		if self.serv:
			try:
				self.serv.privmsg(canal, msg)
			except irclib.ServerNotConnectedError as ex:
				print("send error", ex)

	def send_color(self, canal, msg, *, color=None, background=None, bold=False):
		self.send(canal, add_color(msg, color=color, background=background, bold=bold))
				
	def send_error(self, canal, msg):
		self.send_color(canal, msg, color='white', background="red", bold=True)
		self.send_color(CANAL_ERRORS, msg, background="red", bold=True)

	def send_response(self, canal, id_msg, msg):
		self.send(canal, SEP.join((PREFIX_CMD+"response",str(msg).strip()))+SEP+"id_msg="+str(id_msg))

	def add_cmd_function(self, canal, irc_cmd, cmd_function):
		"""
		Ajouter une commande à la classe en cours.
		@param irc_cmd le nom de la commande
		@param cmd_function la lambda fonction
		"""
		irc_cmd = irc_cmd.lower()
		if canal.startswith("#"):
			canal = canal[1:]
		setattr(self, irc_cmd_to_func_name(canal, irc_cmd), types.MethodType(cmd_function, self))

	def add_executer(self, executer):
		i = id(executer)
		self.executers[i] = executer
		for f_name in filter(lambda s: s.startswith("cmd_"), dir(executer)):
			f = getattr(executer, f_name)
			f_args = inspect.getfullargspec(f).args
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

	def start(self):
		try:
			ircbot.SingleServerIRCBot.start(self)
		except KeyboardInterrupt:
			self.stop()
	
	def stop(self):
		print("exit")
		if self.serv:
			self.serv.disconnect("Tchuss")


