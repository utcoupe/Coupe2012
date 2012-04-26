#!/usr/bin/python3
# -*- coding: utf-8 -*-


from py3irc.mypyirc import MyPyIrcBot



class TestBot(MyPyIrcBot):
	def __init__(self,serverip, serverport):
		MyPyIrcBot.__init__(self, serverip, serverport, "testbot", ["#test"])
		self.canal = "#test"

	def cmd_test_test(self, msg, **kwargs):
		"""
			@param msg un message de votre choix
		"""
		for i in range(16):
			for j in range(16):
				m = chr(3)+str(i)+","+str(j)+ "you said : "+msg+chr(3)+" "+str(i)+","+str(j)
				self.send(self.canal, m)
		return msg

	def cmd_test_error(self, error, **kwargs):
		self.send_error(self.canal, error)

	def cmd__testglob(self, **kwargs):
		self.send(self.canal, "j'ai reçu un appel du canal %s" % kwargs["canal"])


if __name__ == "__main__":
	bot = TestBot("localhost", 6667)
	bot.start()
