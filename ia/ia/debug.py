

from py3irc.mypyirc.ircdefine import *

class Debug:
	def __init__(self, ircbot, canal_debug):
		self.ircbot = ircbot
		self.canal_debug = canal_debug

	def clear_all(self):
		self.send_debug("clear_all")
		
	def draw_circle(self, p, r, color, group, i):
		self.send_debug("draw_circle {x} {y} {radius} {r} {g} {b} {group} {i}".format(
			x=p[0], y=p[1],
			radius=r,
			r=color[0], g=color[1], b=color[2],
			group=group, i=i
		))

	def draw_path(self, path, color, group):
		self.remove_segment_group(group)
		if len(path) > 1:
			last = path[0]
			for i,current in enumerate(path[1:]):
				self.draw_segment(last, current, color, group, i)
				last = current
	
	def draw_segment(self, a, b, color, group, i):
		self.send_debug("draw_segment {x1} {y1} {x2} {y2} {r} {g} {b} {group} {i}".format(
			x1=a[0], y1=a[1],
			x2=b[0], y2=b[1],
			r=color[0], g=color[1], b=color[2],
			group=group, i=i
		))

	def remove_circle(self, group, i):
		self.send_debug("remove_circle %s %s" % (group,i))

	def remove_segment(self, group, i):
		self.send_debug("remove_segment %s %s" % (group,i))

	def remove_segment_group(self, group):
		self.send_debug("remove_segment_group %s" % group)

	def reset(self):
		self.send_debug("reset")
		
	def send_debug(self, msg):
		self.ircbot.send(self.canal_debug, PREFIX_CMD+str(msg))
		
