


from view import *
import time


class GraphView(View):
	def __init__(self, graph):
		View.__init__(self)

		self.graph = graph

		self.p_depart = (200,200)
		self.p_arrive = (1500,1500)

		self.draw_polygons(graph.areas.values())

		self.id_raw_path = None
		self.id_smooth_path = None
		
		## bindings
		self.canvas.bind('<Button-1>',self.onLeft)
		self.canvas.bind('<B1-Motion>',self.onLeft)
		self.canvas.bind('<Button-3>',self.onRight)
		self.canvas.bind('<B3-Motion>',self.onRight)
	
	def onLeft(self, event):
		self.p_arrive = self.event_to_x_y(event)
		self.calc_path()

	def onRight(self, event):
		self.p_depart =  self.event_to_x_y(event)
		self.calc_path()

	def event_to_x_y(self, event):
		return (round(event.x / self.w_to_px), round((HEIGHT - event.y) / self.h_to_px))

	def calc_path(self):
		start = time.time()
		areas,raw_path,smooth_path = self.graph.get_path(self.p_depart,self.p_arrive)
		print("pathfinding computing time : %s " % (time.time() - start))
		if areas:
			self.remove(self.id_raw_path)
			self.remove(self.id_smooth_path)
			self.id_raw_path = self.draw_line(raw_path, 'red')
			self.id_smooth_path = self.draw_line(smooth_path, 'blue')










		
