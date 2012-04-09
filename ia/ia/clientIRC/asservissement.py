



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
	
	def ping(self, **kwargs):
		"""
		@todo gérer le cas où l'on envoi un handler au ping
		"""
		def handler(n, canal, args, options):
			self.e_ping.set()
		self.e_ping.clear()
		return self.send_asserv("ping", handler=handler, **kwargs)
	
	def goto(self, p, vitesse=500, **kwargs):
		return self.send_asserv("goto", p[0], p[1], vitesse, **kwargs)
	
	def gotor(self, p, vitesse=500, **kwargs):
		return self.send_asserv("gotor", p[0], p[1], vitesse, **kwargs)

	def turn(self, a, v=500, **kwargs):
		return self.send_asserv("turn", round(a), v, **kwargs)

	def turnr(self, a, v=500, **kwargs):
		self.send_asserv("turnr", round(a), v, **kwargs)

	def pwm(self, pwm, **kwargs):
		return self.send_asserv("pwm", pwm, **kwargs)

	def stop(self, **kwargs):
		return self.send_asserv("stop", **kwargs)

	def resume(self, **kwargs):
		return self.send_asserv("resume", **kwargs)

	def cancel(self, **kwargs):
		return self.send_asserv("cancel", **kwargs)

	def get_pos(self, **kwargs):
		return self.send_asserv("pos", **kwargs)

	def send_asserv(self, irc_cmd, *args, handler=None, block=False, block_level=1, timeout=None):
		if block:
			return self.send_asserv_block(irc_cmd, *args, handler=handler, block_level=block_level, timeout=timeout)
		else:
			self.ircbot.send_cmd(self.chan_asserv, irc_cmd, *args, handler=handler)
			return True

	def send_asserv_block(self, irc_cmd, *args, handler=None, block_level=1, timeout=None):
		event = threading.Event()
		def f(n, canal, args, kwargs):
			if handler: handler(n, canal, *args, **kwargs)
			print(n,block_level)
			if (n+1) >= block_level:
				event.set()
		self.send_asserv(irc_cmd, *args, handler=f)
		event.wait(timeout)
		return event.is_set()
	




