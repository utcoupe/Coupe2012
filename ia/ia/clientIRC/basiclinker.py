






import threading
import time


from py3irc.mypyirc.ircdefine import *





class BasicLinker:
	def __init__(self, ircbot, channel):
		self.ircbot = ircbot
		self.channel = channel
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
		"""
		def handler(n, canal, args, options):
			self.e_ping.set()
			if 'handler' in kwargs:
				kwargs['handler'](n, canal, args, options)
		self.e_ping.clear()
		return self.send("ping", handler=handler, **kwargs)
	
	def send(self, irc_cmd, *args, handler=None, block=False, block_level=1, timeout=None):
		if block:
			return self.send_block(irc_cmd, *args, handler=handler, block_level=block_level, timeout=timeout)
		else:
			self.ircbot.send_cmd(self.channel, irc_cmd, *args, handler=handler)
			return True

	def send_block(self, irc_cmd, *args, handler=None, block_level=1, timeout=None):
		event = threading.Event()
		def f(n, canal, args, kwargs):
			if handler: handler(n, canal, *args, **kwargs)
			print(n,block_level)
			if (n+1) >= block_level:
				event.set()
		self.send(irc_cmd, *args, handler=f)
		event.wait(timeout)
		return event.is_set()
	




