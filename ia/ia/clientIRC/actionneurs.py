

from .basiclinker import BasicLinker

from geometry import Vec

class ActionneursMini (BasicLinker):
	def __init__(self, ircbot, channel):
		super().__init__(ircbot, channel)

	def arracher_carte(self, **kwargs):
		return self.send("arracher_carte", **kwargs)
