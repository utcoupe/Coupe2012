



class Debug:
	def __init__(self, ircbot, canal_debug):
		self.ircbot = ircbot
		self.canal_debug = canal_debug

	def clear_all(self):
		self.send_debug("clear_all")
		
	def draw_circle(self, p, r, color):
		self.send_debug("draw_circle {x} {y} {radius} {r} {g} {b}".format(
			x=p[0], y=p[1],
			radius=r,
			r=color[0], g=color[1], b=color[2]
		))

	def draw_path(self, path, color):
		self.clear_all()
		last = path[0]
		for current in path[1:]:
			self.draw_segment(last, current, color)
			last = current
	
	def draw_segment(self, a, b, color):
		self.send_debug("draw_segment {x1} {y1} {x2} {y2} {r} {g} {b}".format(
			x1=a[0], y1=a[1],
			x2=b[0], y2=b[1],
			r=color[0], g=color[1], b=color[2]
		))
		
	def send_debug(self, msg):
		self.ircbot.send(self.canal_debug, msg)
