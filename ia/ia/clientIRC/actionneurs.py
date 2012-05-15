

from .basiclinker import BasicLinker

from geometry import Vec

class ActionneursMini (BasicLinker):
	def __init__(self, ircbot, channel):
		super().__init__(ircbot, channel)

	def abaisser_bras(self, **kwargs):
		return self.send("tourner", 0, 0, **kwargs)

	def ouvrir_pince(self, **kwargs):
		return self.send("tourner", 2, 0, **kwargs)

	def fermer_pince(self, **kwargs):
		return self.send("tourner", 2, -105, **kwargs)

	def remonter_bras(self, **kwargs):
		return self.send("remonter_bras", 0, 85, **kwargs)

	def sortir_percepteur_gauche(self, **kwargs):
		return self.send("percepteur", 1, 200, **kwargs)

	def sortir_percepteur_droit(self, **kwargs):
		return self.send("percepteur", 0, 200, **kwargs)

	def rentrer_percepteur_gauche(self, **kwargs):
		return self.send("percepteur", 1, -200, **kwargs)

	def rentrer_percepteur_droit(self, **kwargs):
		return self.send("percepteur", 0, -200, **kwargs)

	def go_buldozer(self, **kwargs):
		return self.send("go_buldozer", **kwargs)

	def stop_buldozer(self, **kwargs):
		return self.send("stop_buldozer", **kwargs)

class ActionneursGros (BasicLinker):
	def __init__(self, ircbot, channel):
		super().__init__(ircbot, channel)

	def fermer_machin(self, **kwargs):
		return self.send("fermer_machin", **kwargs)

	def ouvrir_machin(self, **kwargs):
		return self.send("ouvrir_machin", **kwargs)

	def action_du_troisieme_ax12(self, *kwargs):
		return self.send("action3", **kwargs)
