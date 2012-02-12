

from load import *
from pathfinding import *
from tile import *
from tilesmooth import smooth

W_TILE	= 50

class TiledMap:
	def __init__(self):
		self.obstacles = []
		self.tiles = []
		self.pathfinding = Pathfinding(self.tiles, TILED)
		self.w_tile = 1
		self.m = 0
		self.n = 0

	def load_img(self, filename):
		width,height,collision_matrix = load_img(filename,W_TILE)
		w_tiles = W_TILE
		m,n = int(width / w_tiles), int(height / w_tiles)

		self.w_tile = w_tiles
		self.m = m
		self.n = n
		self.collision_matrix = collision_matrix

		# création des tiles
		for i in range(m):
			self.tiles.append(list([ Tile((i,j), w_tiles) for j in range(n) ]))

		# initialisation (position + voisins)
		for i in range(m):
			for j in range(n):
				neighbors = []
				if i > 0:
					neighbors.append(self.tiles[i-1][j])
					if j > 0:
						neighbors.append(self.tiles[i-1][j-1])
					if j+1 < n:
						neighbors.append(self.tiles[i-1][j+1])
				if i+1 < m:
					neighbors.append(self.tiles[i+1][j])
					if j > 0:
						neighbors.append(self.tiles[i+1][j-1])
					if j+1 < n:
						neighbors.append(self.tiles[i+1][j+1])
				if j > 0:
					neighbors.append(self.tiles[i][j-1])
				if j+1 < n:
					neighbors.append(self.tiles[i][j+1])
				x,y = i*w_tiles+w_tiles//2,j*w_tiles+w_tiles//2
				self.tiles[i][j].init(i * n + j, (x,y), neighbors)
				self.tiles[i][j].walkable = not collision_matrix[i][j]
				if collision_matrix[i][j]:
					self.tiles[i][j].color = 'black'

	def get_path(self, p_depart, p_arrive):
		tile_depart = self.get_node_from_pos(p_depart)
		tile_arrive = self.get_node_from_pos(p_arrive)
		tiles, raw_path = self.pathfinding.compute_path(tile_depart, tile_arrive)
		if len(tiles) > 1:
			smooth_path = smooth(raw_path, self.tiles, W_TILE)
		else:
			raw_path = [p_depart, p_arrive]
			smooth_path = [p_depart, p_arrive]
		return tiles,raw_path,smooth_path

	def get_node_from_pos(self, p):
		return self.tiles[p[0]//self.w_tile][p[1]//self.w_tile]

	def get_polygons(self):
		r = []
		for l in self.tiles:
			r.extend(l)
		return r
		
	def update(self):
		""" fonction à appeller après avoir bougé un obstacle dynamic """
		#todo
		pass
	
if __name__ == "__main__":
	import time
	import sys
	filename = sys.argv[1]
	tm = TiledMap()
	start = time.time()
	tm.load_img(filename)
	print("img load time : %s" % (time.time() - start))
	
	sys.path.append("../view")
	from graphview import *
	
	dynamic_obstacle = Poly().initFromCircle((-500,-500),250,8)
	v = GraphView(tm,dynamic_obstacle)
	v.mainloop()			
