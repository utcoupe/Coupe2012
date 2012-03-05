

import pygame.image as im
import xml.dom.minidom


class CollisionMatrix:
	def __init__(self):
		self._matrix = []
		self.rectangles = []

	def load_img(self, filename, scale=1, seuil=120):
		surf = im.load(filename)
		img_width = surf.get_width()
		img_height = surf.get_height()
		self._matrix = [ [ False if surf.get_at((x,y)).r > seuil else True for y in range(0,img_height,scale) ] for x in range(0,img_width,scale) ]

	def load_xml(self, filename, scale=1):
		with open(filename) as f:
			document = f.read()
			f.close()
	
			dom = xml.dom.minidom.parseString(document)

			# size
			size = dom.getElementsByTagName("size")[0]
			width = round(int(size.getAttribute("width"))/scale)
			height = round(int(size.getAttribute("height"))/scale)
			
			# init empty matrix
			self._matrix = [ [ False for _ in range(height) ] for __ in range(width) ]

			# rectangles
			for rec in dom.getElementsByTagName("rectangle"):
				x1,y1 = int(rec.getAttribute("x1")),int(rec.getAttribute("y1"))
				x2,y2 = int(rec.getAttribute("x2")),int(rec.getAttribute("y2"))
				x1,x2 = min(x1,x2),max(x1,x2)
				y1,y2 = min(y1,y2),max(y1,y2)
				self.rectanles.append((x1,y1,x2,y2))
				for x in range(x1,x2,scale):
					for y in range(y1,y2,scale):
						self._matrix[round(x/scale)][round(y/scale)] = True
	
	def collide(self, p):
		return self._matrix[p[0]][p[1]]

	def width(self):
		return len(self._matrix)

	def height(self):
		return 0 if not self._matrix else len(self._matrix[0])
