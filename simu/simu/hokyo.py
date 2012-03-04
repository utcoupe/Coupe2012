
import sys
import os
sys.path.append(os.path.join("..","lib","py3irc"))


from mypyircbot import *


class Hokuyo(Executer):
	def __init__(self, canal, robots):
		Executer.__init__(self)
		self.canal = canal
		self.robots = robots

	def cmd_hokuyo_get(self, **kwargs):
		reponse = map(lambda r: '(%s,%s)' % (r.x(),r.y()), self.robots)
		self.send(self.compute_msg(kwargs['id_msg'], *reponse))

	def cmd_hokuyo_ping(self, **kwargs):
		self.send(self.compute_msg(kwargs['id_msg'], "pong"))

	def send(self, msg):
		Executer.send(self, self.canal, msg)
