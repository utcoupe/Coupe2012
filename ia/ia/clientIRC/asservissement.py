



import threading
import time


from py3irc.mypyirc.ircdefine import *


from geometry import Vec



class Asservissement:
	def __init__(self, ircbot, chan_asserv):
		self.ircbot = ircbot
		self.pos = Vec((0,0))
		self.angle = 0
		self.chan_asserv = chan_asserv
		self.last_update_pos = 0
		self.e_ping = threading.Event()

	def get_latency(self):
		n = 10
		start = time.time()
		for i in range(n):
			self.ping()
			self.e_ping.wait()
		return (time.time() - start) / n
	
	def ping(self):
		def handler(canal,args, options):
			self.e_ping.set()
		self.e_ping.clear()
		self.send_asserv("ping", handlers=[handler])
	
	def goto(self, p, vitesse=500):
		def handler(canal,args,options):
			print("received from asserv : %s, %s" %(args,options))
		self.send_asserv("goto", p[0], p[1], vitesse, handlers=[handler])

	def turn(self, a, v):
		self.send_asserv("turn", round(a), v)

	def pwm(self, pwm):
		self.send_asserv("pwm", pwm)

	def stop(self):
		self.send_asserv("stop")

	def resume(self):
		self.send_asserv("resume")

	def cancel(self):
		self.send_asserv("cancel")

	def get_pos(self, handler):
		self.send_asserv("pos", handlers=[handler])

	def send_asserv(self, irc_cmd, *args, handlers=[]):
		self.ircbot.send_cmd(self.chan_asserv, handlers, irc_cmd, *args)

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

		




