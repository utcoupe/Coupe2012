
from .basiclinker import BasicLinker



class Hokuyo(BasicLinker):
	def __init__(self, ircbot, channel):
		super().__init__(ircbot, channel)
		
	def get(self, **kwargs):
		self.send("get", **kwargs)
