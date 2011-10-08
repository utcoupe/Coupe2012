# -*- coding: utf-8 -*-



import irclib
import ircbot



class MyIRCBot(ircbot.SingleServerIRCBot):
	def __init__(self, server_ip, server_port, nickname, channel):
		self.serv = None
		self.nickname = nickname
		self.channel = channel
		
		ircbot.SingleServerIRCBot.__init__(self, [(server_ip, server_port)],
										   nickname, "Bot réalisé en Python avec ircbot")
	
	def on_nicknameinuse(self, serv, e):
		self.nickname += "_"
		serv.nick(self.nickname + "_")
	
	def on_welcome(self, serv, ev):
		"""
		Méthode appelée une fois connecté et identifié.
		Notez qu'on ne peut rejoindre les canaux auparavant.
		"""
		serv.join(self.channel)
		self.serv = serv
		
	def on_pubmsg(self, serv, ev):
		"""
		Méthode appelée à la réception d'un message, qui exclut son expéditeur s'il
		écrit une insulte.
		"""
		self.serv = serv
		
		auteur = irclib.nm_to_n(ev.source())
		canal = ev.target()
		msg = ev.arguments()[0].strip().lower()
		msg_split = msg.split(" ")
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
			if len(msg_split)-1 == len(inspect.getargspec(f).args):
				msg = bytes(f(*msg_split[1:])+"\n","utf-8")
				print (msg)
				self.serial.write(msg)
			else:
				serv.privmsg(canal, "invalid arg number : need %s and get %s" % (str(inspect.getargspec(f)),msg_split))

	def print_doc(self, f_name, cmd=None):
		cmd = f_name if not cmd else cmd
		try:
			doc = getattr(self, f_name).__doc__
		except AttributeError as ex:
			self.serv.privmsg(self.channel, str(ex))
		else:
			if not doc: doc = "No documentation"
			for line in doc.split("\n"):
				self.serv.privmsg(self.channel, cmd + ":" + line)
	
