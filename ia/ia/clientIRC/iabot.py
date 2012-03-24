# -*- coding: utf-8 -*-




import threading
import time

from py3irc.mypyirc import MyPyIrcBot
from py3irc.mypyirc.ircdefine import *


class IABot(MyPyIrcBot):
	def __init__(self, server_ip, server_port, *,
			canal_big_asserv, canal_mini_asserv, canal_big_others, canal_mini_others, canal_debug, canal_hokuyo):
		MyPyIrcBot.__init__(self,
			server_ip, server_port,
			"iabot",
			[canal_big_asserv, canal_mini_asserv, canal_big_others, canal_mini_others, canal_debug, canal_hokuyo]
		)


		self.__id = 0
		self.__id_lock = threading.Lock()

		self.handlers = {}

	def get_new_id(self):
		"""
		Retourne un identifiant unique
		"""
		self.__id_lock.acquire()
		if self.__id > 1E12:
			self.__id = 0
		i = self.__id
		self.__id += 1
		self.__id_lock.release()
		return i

	def cmd__response(self, *args, canal, id_msg, **options):
		id_msg = int(id_msg)
		if id_msg in self.handlers:
			n, handler = self.handlers[id_msg]
			self.handlers[id_msg][0] += 1
			handler(n, canal, args, options)
	
	def send_cmd(self, canal, irc_cmd, *args, handler=None):
		"""
		@param canal
		@param handlers, list de handlers (=fonctions appellée succéssivement lors de la reception de la réponse)
		@param irc_cmd la commande (ex: goto)
		@param les arguments de la commande (ex: [400,300,1000])
		"""
		if not irc_cmd.startswith(PREFIX_CMD): irc_cmd = PREFIX_CMD+irc_cmd
		i = self.get_new_id()
		if handler:
			self.set_handler(i, handler)
		str_args = map(str, (irc_cmd,)+args)
		self.send(canal, SEP.join(str_args) + SEP + "id_msg=%s" % i)

	def set_handler(self, id_msg, handler, n=0):
		self.handlers[int(id_msg)] = [n, handler]
		

if __name__ == "__main__":
	from random import randrange
	import time
	bot = IABot("localhost", 6667, "#asserv", "#asservmini")
	def bidon():
		while True:
			x = randrange(0,3000)
			y = randrange(0,2000)
			bot.send_msg("#asserv", "cancel")
			bot.send_msg("#asservmini", "goto %s %s 2000" % (x,y))
			time.sleep(3)
	t = threading.Thread(None, bidon, "send")
	t.setDaemon(True)
	t.start()
	bot.start()



	

