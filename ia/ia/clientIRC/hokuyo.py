
import time
import threading

from geometry import Vec


class Hokuyo:
	def __init__(self, ircbot, chan_hokuyo):
		self.ircbot = ircbot
		self.pos = Vec((0,0))
		self.angle = 0
		self.chan_hokuyo = chan_hokuyo
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
		self.send_hokuyo("ping", handler=handler)
		
	def get(self, *, handler):
		self.send_hokuyo("get", handler=handler)


	def send_hokuyo(self, irc_cmd, *args, handler=None):
		self.ircbot.send_cmd(self.chan_hokuyo, irc_cmd, *args, handler=handler)
	
