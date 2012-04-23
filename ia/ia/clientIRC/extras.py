

from .basiclinker import BasicLinker





class Extras(BasicLinker):
	def __init__(self, ircbot, channel):
		super().__init__(ircbot, channel)

	def teleport(self, p, a, **kwargs):
		self.send("teleport", p[0], p[1], a, **kwargs)
	
