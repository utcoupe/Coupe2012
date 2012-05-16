

from py3irc.mypyirc.mypyircbot import Executer


class Hokuyo(Executer):
	def __init__(self, canal, robots):
		Executer.__init__(self)
		self.canal = canal
		self.robots = robots

	def cmd_hokuyo_getdata(self, **kwargs):
		lr = filter(lambda r: r.x() > 0 and r.y() > 0, self.robots)
		reponse = str(tuple(map(lambda r: (r.x(),r.y()), self.robots)))
		reponse = reponse.replace(" ","")
		self.send_canal_hokuyo(kwargs['id_msg'], reponse)

	def cmd_hokuyo_ping(self, **kwargs):
		self.send_canal_hokuyo(kwargs['id_msg'], "pong")

	def send_canal_hokuyo(self, id_msg, msg):
		self.send_response(self.canal, id_msg, msg)
