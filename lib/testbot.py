

from py3irc.mypyirc import MyPyIrcBot



class TestBot(MyPyIrcBot):
	def __init__(self,serverip, serverport):
		MyPyIrcBot.__init__(self, serverip, serverport, "testbot", ["#test"])
		self.canal = "#test"

	def cmd_test_test(self, msg, **kwargs):
		self.send(self.canal, msg)
		return msg


if __name__ == "__main__":
	bot = TestBot("grosordi.cheuv.eu", 6667)
	bot.start()
