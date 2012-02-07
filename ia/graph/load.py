


import pygame.image as im
import xml.dom.minidom


from geometry.poly import *

def load_img(filename, seuil=125):
	surf = im.load(filename)
	img_width = surf.get_width()
	img_height = surf.get_height()
	matrix = [ [ False if surf.get_at((x,y)).r > seuil else True for y in range(img_height) ] for x in range(img_width) ]
	return img_width, img_height, matrix

def load_xml(filename):
	obstacles = []
	with open(filename) as f:
		document = f.read()
		f.close()

		dom = xml.dom.minidom.parseString(document)

		# size
		size = dom.getElementsByTagName("size")[0]
		width = int(size.getAttribute("width"))
		height = int(size.getAttribute("height"))

		#  polygon
		for xml_poly in dom.getElementsByTagName("polygon"):
			obstacles.append(xml_to_polygon(xml_poly))

		# carre
		for xml_carre in dom.getElementsByTagName("carre"):
			obstacles.append(xml_to_carre(xml_carre))

		# cercle
		for xml_circle in dom.getElementsByTagName("circle"):
			obstacles.append(xml_to_circle(xml_circle))

	return width, height, obstacles
		

def xml_to_polygon(xml_poly):
	poly = Poly(((int(vertex.getAttribute("x")), int(vertex.getAttribute("y"))) for vertex in xml_poly.getElementsByTagName("vertex")))
	return poly

def xml_to_carre(xml_carre):
	x,y,w = int(xml_carre.getAttribute("x")), int(xml_carre.getAttribute("y")), int(xml_carre.getAttribute("width"))
	poly = Poly().initFromCarre((x,y), w, True)
	return poly

def xml_to_circle(xml_circle):
	x,y,r,n = int(xml_circle.getAttribute("x")), int(xml_circle.getAttribute("y")), int(xml_circle.getAttribute("rayon")), int(xml_circle.getAttribute("n"))
	poly = Poly().initFromCircle((x,y), r, n)
	return poly


