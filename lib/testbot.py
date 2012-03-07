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
		self.send(self.canal, "you said : "+msg)
		return msg


if __name__ == "__main__":
	bot = TestBot("localhost", 6667)
	bot.start()
