
from .basiclinker import BasicLinker


import math

class Visio(BasicLinker):
	def __init__(self, ircbot, channel):
		super().__init__(ircbot, channel)
		
	def get(self, **kwargs):
		self.send("get", **kwargs)

	@staticmethod
	def relative_to_absolute(pos_robot, angle_robot, points):
		print("COUCOU", points)
		cosa = math.cos(angle_robot)
		sina = math.sin(angle_robot)
		def f(p):
			x = cosa * p[0] - sina * p[1] + pos_robot[0]
			y = sina * p[0] + cosa * p[1] + pos_robot[1]
			return (x,y)
		return tuple(map(f,points))
	
