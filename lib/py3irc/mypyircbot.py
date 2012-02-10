# -*- coding: utf-8 -*-
"""
	@author: Thomas Recouvreux
"""

import inspect
import re

import irclib
import ircbot



class MyPyIrcBot(ircbot.SingleServerIRCBot):
	def __init__(self, server_ip, server_port, nickname, channel):
		self.serv = None
		self.nickname = nickname
		self.channel = channel
		
		ircbot.SingleServerIRCBot.__init__(self,
			[(server_ip, server_port)],
			nickname,
			"Bot réalisé en Python avec ircbot",
			1
		)
	
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
		serv.join(self.channel)
		self.serv = serv
	
	def write_rep(self, msg, id_msg=42):
		"""
		Fonction qui redistribue le message retourné par les fonctions "cmd_*".
		@param msg le message à envoyer
		"""
		self.send("%s (id=%s)" %(msg,id_msg))
		
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
		raw_msg = ev.arguments()[0]
		if '#' in raw_msg:
			msg, str_options = raw_msg.split('#')
			msg.strip()
			str_options.strip()
		else:
			msg, str_options = raw_msg, ""
		msg_split = msg.strip().split(" ")
		spec_id = "id=(?P<id>[0-9]+)"
		t = re.search(spec_id, str_options)
		if t:
			id_msg = t.group("id")
		else:
			id_msg = 42
		f_name = "cmd_" + msg_split[0]
		if msg_split[0] == "help":
			if len(msg_split) > 1:
				self.print_doc("cmd_"+msg_split[1], msg_split[1])
			else:
				for f_name in dir(self):
					if "cmd_" == f_name[0:4]:
						self.print_doc(f_name, f_name[4:])
		elif hasattr(self, f_name):
			f = getattr(self, f_name)
			f_args = inspect.getargspec(f).args
			nb_args = len(f_args) - (1 if 'self' in f_args else 0)
			if len(msg_split)-1 == nb_args:
				self.write_rep(f(*msg_split[1:])+"\n",id_msg)
			else:
				serv.privmsg(canal, "invalid arg number : need %s and get %s" % (str(inspect.getargspec(f)),msg_split))

	def print_doc(self, f_name, cmd=None):
		"""
		Afficher la doc d'une fonction,
		ex : print_doc(cmd_bidule, bidule) =>
		"bidule : voici la documentation de bidule"
		@param f_name le nom interne de la fonction
		@param cmd le nom à afficher
		"""
		cmd = f_name if not cmd else cmd
		try:
			doc = getattr(self, f_name).__doc__
		except AttributeError as ex:
			self.serv.privmsg(self.channel, str(ex))
		else:
			if not doc: doc = "No documentation"
			for line in doc.split("\n"):
				self.serv.privmsg(self.channel, cmd + ":" + line)

	def send(self, msg):
		"""
		Envoie un message au serveur.
		@param msg le message à envoyer
		"""
		if self.serv: self.serv.privmsg(self.channel, msg)
