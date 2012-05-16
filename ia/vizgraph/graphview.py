# -*- coding: utf-8 -*-


from view import *
import time


class GraphView(View):
	def __init__(self, graph, dynamic_obstacle):
		View.__init__(self)

		self.graph = graph

		self.p_depart = (200,200)
		self.p_arrive = (1500,1500)

		self.draw_polygons(graph.get_polygons())

		self.id_raw_path = None
		self.id_smooth_path = None

		self.dynamic_obstacle = dynamic_obstacle
		
		## bindings
		self.canvas.bind('<Button-1>',self.onLeft)
		self.canvas.bind('<B1-Motion>',self.onLeft)
		self.canvas.bind('<Button-3>',self.onRight)
		self.canvas.bind('<B3-Motion>',self.onRight)
		self.canvas.bind('<Button-2>',self.onWheel)
		self.canvas.bind('<B2-Motion>',self.onWheel)

		self.sum_calc_times = 0
		self.nb_calc_times = 0
		self.sum_update = 0
		self.nb_update = 0
	
	def onLeft(self, event):
		event = self.event_to_x_y(event)
		print(event)
		self.p_arrive = event
		self.calc_path()

	def onRight(self, event):
		event = self.event_to_x_y(event)
		print(event)
		self.p_depart =  event
		self.calc_path()

	def onWheel(self, event):
		event = self.event_to_x_y(event)
		print(event)
		
		start = time.time()
		self.dynamic_obstacle.move_to(event)
		self.graph.update()
		difference = (time.time() - start)
		self.sum_update += difference
		self.nb_update += 1
		print("update graph time : %s (%s)" % (difference,self.sum_update / self.nb_update))
		
		self.remove()
		self.draw_polygons(self.graph.get_polygons())
		self.calc_path()

	def event_to_x_y(self, event):
		return (round(event.x / self.w_to_px), round((HEIGHT - event.y) / self.h_to_px))

	def calc_path(self):
		start = time.time()
		nodes,raw_path,smooth_path = self.graph.get_path(self.p_depart,self.p_arrive)
		difference = (time.time() - start)
		self.sum_calc_times += difference
		self.nb_calc_times += 1
		print("pathfinding computing time : %s (moy=%s)" % (difference,self.sum_calc_times/self.nb_calc_times))
		print(nodes,raw_path,smooth_path)

		self.show_result_calc_path(nodes,raw_path,smooth_path)
		
		return nodes,raw_path,smooth_path

	def show_result_calc_path(self, nodes,raw_path,smooth_path):
		self.remove(self.id_raw_path)
		self.remove(self.id_smooth_path)
		if smooth_path:
			self.id_raw_path = self.draw_line(raw_path, 'red')
			self.id_smooth_path = self.draw_line(smooth_path, 'blue')







		
