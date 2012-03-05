
import tkinter as tk


from navgraph import *



class Window(tk.Tk):
	def __init__(self, filename):
		tk.Tk.__init__(self,None)
		self.grid()
		self.cvw = 600
		self.cvh = 400
		self.canvas = tk.Canvas(self, width=self.cvw, height=self.cvh, background='white')
		self.canvas.grid(row=0, column=0)
		self.buttonsFrame = tk.Frame(self, relief= "solid", bd= 2)

		self.navgraph = NavGraph()
		self.navgraph.generate_from_xml(filename, 50)
		#self.navgraph.generate_from_img(filename,10)
		self.print_collision_matrix()

	def print_collision_matrix(self):
		tile_width = round(self.cvw / self.navgraph.collision_matrix.width())
		tile_height = round(self.cvh / self.navgraph.collision_matrix.height())
		for i in range(0,self.navgraph.collision_matrix.width()):
			for j in range(0,self.navgraph.collision_matrix.height()):
				color = 'gray' if self.navgraph.collision_matrix.collide((i,j)) else 'white'
				self.canvas.create_rectangle(i*tile_width,j*tile_height,(i+1)*tile_width,(j+1)*tile_height, fill=color)
		


w = Window('map.xml')
w.mainloop()

		
