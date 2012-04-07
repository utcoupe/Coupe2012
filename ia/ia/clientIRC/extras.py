

from py3irc.mypyirc.ircdefine import *





class Extras:
	def __init__(self, ircbot, chan_extras):
		self.ircbot = ircbot
		self.chan_extras = chan_extras

	def teleport(self, p, a, handler=None):
		self.send_extras("teleport", p[0], p[1], a, handler=handler)
	
	def send_extras(self, irc_cmd, *args, handler=None):
		self.ircbot.send_cmd(self.chan_extras, irc_cmd, *args, handler=handler)

