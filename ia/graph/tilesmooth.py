
from copy import copy

from geometry.vec import *


UP			= 0
DOWN		= 1
LEFT		= 2
RIGHT		= 3
UP_LEFT 	= 4
UP_RIGHT	= 5
DOWN_LEFT	= 6
DOWN_RIGHT	= 7

def smooth(raw_path, tiles, tile_size):
	raw_path = [ Vec(p) for p in raw_path ]
	smooth_path = []
	if len(raw_path) > 2:
		new_path = [ Vec(p) for p in raw_path ]
		check_point = raw_path[0]
		current = raw_path[1]
		check_point_dir = dir_to(check_point,current)
		for node in raw_path[2:]:
			if walkable(check_point,node,check_point_dir, tiles, tile_size):
				new_path.remove(current)
			else:
				check_point = current
				check_point_dir = dir_to(check_point,node)
			current = node
		smooth_path = new_path
	else:
		smooth_path = raw_path

	return smooth_path

def walkable(node1, node2, current_dir, tiles, tile_size):
	"""
	optimisation supprimmée parce que elle fait passer le path dans des murs,
	la fonction doit être buggée
	if current_dir:
		d = dir_to(node1,node2)
		if d == current_dir:
			return True"""
	m = node2 - node1
	length = round(m.norm())+1
	m.normalize()
	point = Vec(node1)
	for t in range(1,length,4):
		new_point = point + (m * t)
		x,y = new_point
		if not tiles[int(x/tile_size)][int(y/tile_size)].walkable:
			return False
	return True

def dir_to(self,other):
	if other[0] == self[0]:
		if other[1] > self[1]:
			return DOWN
		else:
			return UP
	elif other[1] == self[1]:
		if other[0] > self[0]:
			return RIGHT
		else:
			return LEFT
	else:
		v = other - self
		if v[0] == v[1]:
			if v[0] > 0:
				return DOWN_RIGHT
			else:
				return UP_LEFT
		elif v[0] == -v[1]:
			if v[0] > 0:
				return UP_RIGHT
			else:
				return DOWN_LEFT
					
