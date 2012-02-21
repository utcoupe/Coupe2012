
import time
import re

from geometry.vec import *


SEP = '.'

ID_MSG_POS		= -1

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

	def turn(self, a, v):
		self.send_asserv("turn %s %s" % (round(a),v))

	def pwm(self, pwm):
		self.send_asserv("pwm %s" % pwm)

	def stop(self):
		self.send_asserv("stop")

	def resume(self):
		self.send_asserv("resume")

	def cancel(self):
		self.send_asserv("cancel")

	def refresh_pos(self):
		self.send_asserv("pos # id=%s" % ID_MSG_POS)

	def send_asserv(self, msg):
		self.ircbot.send(self.chan_asserv, msg)

	def on_msg(self, canal, auteur, msg):
		if canal == self.chan_asserv:
			msg_split = msg.split(SEP)
			if len(msg_split) > 2:
				id_msg = int(msg_split[0])
				params = msg_split[1:]
				
				if ID_MSG_POS == id_msg:
					self.on_pos_recv(params)
					
	def on_pos_recv(self, params):
		if len(params) == 3:
			self.pos[0] = int(params[0])
			self.pos[1] = int(params[1])
			self.angle = int(params[2])
			self.last_update_pos = time.time()
		else:
			print("ERROR on_pos_recv : pas assez de paramètres, '%s'" % (params,))

		




