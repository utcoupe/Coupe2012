



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
		def handler(n, canal, args, options):
			self.e_ping.set()
		self.e_ping.clear()
		self.send_asserv("ping", handler=handler)
	
	def goto(self, p, vitesse=500, *, handler=None):
		self.send_asserv("goto", p[0], p[1], vitesse, handler=handler)

	def turn(self, a, v, *, handler=None):
		self.send_asserv("turn", round(a), v, handler=handler)

	def pwm(self, pwm, *, handler=None):
		self.send_asserv("pwm", pwm, handler=handler)

	def stop(self, *, handler=None):
		self.send_asserv("stop", handler=handler)

	def resume(self, *, handler=None):
		self.send_asserv("resume", handler=handler)

	def cancel(self, *, handler=None):
		self.send_asserv("cancel", handler=handler)

	def get_pos(self, *, handler=None):
		self.send_asserv("pos", handler=handler)

	def send_asserv(self, irc_cmd, *args, handler=None):
		self.ircbot.send_cmd(self.chan_asserv, irc_cmd, *args, handler=handler)
	




