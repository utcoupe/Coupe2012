
import time

from geometry.vec import *
from graph import *

SEP = '.'

class Robot:
	def __init__(self, ircbot, chan_asserv):
		self.ircbot = ircbot
		ircbot.add_listener(self.on_msg)
		self.pos = Vec((0,0))
		self.angle = 0
		self.chan_asserv = chan_asserv
		self.last_update_pos = 0

	def goto(self, p, vitesse=500):
		self.send_asserv("goto %s %s %s" % (p[0], p[1], vitesse))

	def stop(self):
		self.send_asserv("stop")

	def resume(self):
		self.send_asserv("resume")

	def cancel(self):
		self.send_asserv("cancel")

	def update_pos(self):
		self.send_asserv("pos 333")

	def send_asserv(self, msg):
		self.ircbot.send_msg(self.chan_asserv, msg)

	def on_msg(self, canal, auteur, msg):
		msg_split = msg.split(SEP)
		if len(msg_split) > 2:
			id_msg = msg_split[0]
			params = msg_split[1:]
			if "333" == id_msg and len(params) == 3:
				self.pos[0] = int(params[0])
				self.pos[1] = int(params[1])
				self.angle = int(params[2])
				self.last_update_pos = time.time()
				print("UPDATE POS", self.pos)
				






