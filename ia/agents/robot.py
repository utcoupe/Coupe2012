
from geometry.vec import *

class Robot:
	def __init__(self, ircbot, chan_asserv):
		self.ircbot = ircbot
		self.pos = Vec((0,0))
		self.chan_asserv = chan_asserv

	def goto(self, p, vitesse=500):
		self.send_asserv("goto %s %s %s" % (p[0], p[1], vitesse))

	def stop(self):
		self.send_asserv("stop")

	def resume(self):
		self.send_asserv("resume")

	def cancel(self):
		self.send_asserv("cancel")

	def ask_pos(self):
		self.send_asserv("pos")

	def send_asserv(self, msg):
		self.ircbot.send_msg(self.chan_asserv, msg)







