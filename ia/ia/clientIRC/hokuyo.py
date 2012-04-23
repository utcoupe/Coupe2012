
from .basiclinker import BasicLinker

from geometry import Vec


class Hokuyo(BasicLinker):
	def __init__(self, ircbot, channel):
		super().__init__(ircbot, channel)
		self.pos = Vec((0,0))
		self.angle = 0
		self.last_update_pos = 0
		
	def get(self, **kwargs):
		self.send("get", **kwargs)
