# -*- coding: utf-8 -*-
"""
	@author: Thomas Recouvreux
"""

import inspect
import re

import irclib
import ircbot

SEP = '.'


def raw_msg_to_msg_n_options(raw_msg):
	"""
	
	"""
	raw_msg = raw_msg.strip().lower()
	if '#' in raw_msg:
		msg, str_options = raw_msg.split('#')
		msg.strip()
		str_options.strip()
	else:
		msg, str_options = raw_msg, ""
		
	options = {
		'id_msg': 42
	}
	specs = {
		'id_msg': "id=(?P<id_msg>[0-9]+)"
	}
	if str_options:
		for i,spec in specs.items():
			t = re.search(spec, str_options)
			if t:
				options[i] = t.group(i)

	return msg, options


class MyPyIrcBot(ircbot.SingleServerIRCBot):
	def __init__(self, server_ip, server_port, nickname, channels):
		self.serv = None
		self.nickname = nickname
		self.canaux = channels
		
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
		for chan in self.canaux:
			serv.join(chan)
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
		canal = ev.target().strip().lower()[1:]
		msg, options = raw_msg_to_msg_n_options(ev.arguments()[0])
		msg_split = msg.strip().split(" ")
		f_name = self.irc_cmd_to_func_name(canal, msg_split[0])
		if msg_split[0] == "help":
			if len(msg_split) > 1:
				self.print_doc(self.irc_cmd_to_func_name(canal, msg_split[1]))
			else:
				for f_name in dir(self):
					if self.channel_to_prefix_cmd(canal) in f_name:
						self.print_doc(f_name)
		elif hasattr(self, f_name):
			f = getattr(self, f_name)
			f_args = inspect.getargspec(f).args
			nb_args = len(f_args) - (1 if 'self' in f_args else 0)
			if len(msg_split)-1 == nb_args:
				self.write_rep(f(*msg_split[1:])+"\n",options['id_msg'])
			else:
				serv.privmsg(canal, "invalid arg number : need %s and get %s" % (str(inspect.getargspec(f)),msg_split))
		
	
	def print_doc(self, f_name):
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
			self.serv.privmsg(self.channel, str(ex))
		else:
			if not doc: doc = "No documentation"
			irc_cmd = self.func_name_to_irc_cmd(f_name)
			for line in doc.split("\n"):
				self.serv.privmsg(self.channel, irc_cmd + ":" + line)

	def send(self, msg):
		"""
		Envoie un message au serveur.
		@param msg le message à envoyer
		"""
		if self.serv: self.serv.privmsg(self.channel, msg)


	def get_protocole(self, str_protocole, prefix):
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
		@param prefixes les prefixes des define
		@return une liste de dictionnaires {id: ?, name: ?, params: ?, doc: ?}
		"""
		
		commands = []

		# spec des regexp
		spec_doc = '\/\*\*(?P<doc>(.(?!\*\/))*.)\*\/'
		spec_define = '#define\s+{prefix}(?P<name>\w+)\s+(?P<id>\d+)'.format(prefix=prefix)
		spec_cmd = spec_doc+"\s"+spec_define
		spec_params = '@param\s+(?P<param>[a-zA-Z_]\w*)'

		# compilation de la regexp pour les paramètres
		re_params = re.compile(spec_params)

		# recherche des commandes
		for t in re.finditer(spec_cmd,str_protocole,re.DOTALL):
			params = list([p.group("param") for p in re_params.finditer(t.group('doc'))])
			commands.append({'id': int(t.group('id')), 'name': t.group('name'), 'params': params, 'doc': t.group('doc')})
			print(commands[-1])

		return commands
		

	def get_protocole_multi_prefixes(self, str_protocole, prefixes):

		# initialisation du dictionnaire
		commands = {}
		for prefix in prefixes:
			commands[prefix] = []

		for prefix in prefixes:
			commands[prefix] = self.get_protocole(str_protocole, prefix)

		return commands
		

	def channel_to_prefix_cmd(self, canal):
		return "cmd_%s_" % canal
	
	def irc_cmd_to_func_name(self, canal, irc_cmd):
		return self.channel_to_prefix_cmd(canal) + irc_cmd.lower()

	def func_name_to_irc_cmd(self, f_name):
		return f_name.split('_',2)[2]

	def add_cmd_function(self, irc_cmd, cmd_function):
		"""
		Ajouter une commande à la classe en cours.
		@param irc_cmd le nom de la commande
		@param cmd_function la lambda fonction
		"""
		setattr(self, self.irc_cmd_to_func_name(irc_cmd), cmd_function)






