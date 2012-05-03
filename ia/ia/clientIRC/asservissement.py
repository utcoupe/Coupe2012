





from .basiclinker import BasicLinker


from geometry import Vec



class Asservissement(BasicLinker):
	def __init__(self, ircbot, channel):
		super().__init__(ircbot, channel)
		self.pos = Vec((0,0))
		self.angle = 0
		self.last_update_pos = 0

	def goto(self, p, vitesse=500, **kwargs):
		return self.send("goto", p[0], p[1], vitesse, **kwargs)
	
	def gotor(self, p, vitesse=500, **kwargs):
		return self.send("gotor", p[0], p[1], vitesse, **kwargs)

	def turn(self, a, v=500, **kwargs):
		return self.send("turn", round(a), v, **kwargs)

	def turnr(self, a, v=500, **kwargs):
		self.send("turnr", round(a), v, **kwargs)

	def pwm(self, pwm, **kwargs):
		return self.send("pwm", pwm, **kwargs)

	def stop(self, **kwargs):
		return self.send("stop", **kwargs)

	def resume(self, **kwargs):
		return self.send("resume", **kwargs)

	def cancel(self, **kwargs):
		return self.send("cancel", **kwargs)

	def get_pos(self, **kwargs):
		return self.send("pos", **kwargs)

	def set_pos(self, p, **kwargs):
		return self.send("set_pos", **kwargs)




