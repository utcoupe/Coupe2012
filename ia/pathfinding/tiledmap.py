

from load import *
from pathfinding import *
from tile import *

W_TILES	= 50

class TiledMap:
	def __init__(self):
		self.obstacles = []
		self.tiles = []
		self.pathfinding = Pathfinding(self.tiles, TILED)
		self.w_tiles = 1
		self.m = 0
		self.n = 0

	def load_img(self, filename):
		width,height,collision_matrix = load_img(filename,W_TILES)
		w_tiles = W_TILES
		m,n = width // w_tiles, height // w_tiles

		self.w_tiles = w_tiles
		self.m = m
		self.n = n

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
				self.tiles[i][j].init(i * n + j, (i*w_tiles+w_tiles/2, j*w_tiles+w_tiles/2), neighbors)
				self.tiles[i][j].walkable = not collision_matrix[i][j]
				if collision_matrix[i][j]:
					self.tiles[i][j].color = 'black'

			

	def get_path(self, p_depart, p_arrive):
		tile_depart = self.tiles[p_depart[0]//self.w_tiles][p_depart[1]//self.w_tiles]
		tile_arrive = self.tiles[p_arrive[0]//self.w_tiles][p_arrive[1]//self.w_tiles]
		tiles, raw_path = self.pathfinding.compute_path(tile_depart, tile_arrive)
		if len(tiles) > 1:
			smooth_path = raw_path
		else:
			raw_path = [p_depart, p_arrive]
			smooth_path = [p_depart, p_arrive]
		return tiles,raw_path,smooth_path

	def get_polygons(self):
		r = []
		for l in self.tiles:
			r.extend(l)
		return r
	
if __name__ == "__main__":
	import time
	import sys
	filename = sys.argv[1]
	tm = TiledMap()
	start = time.time()
	tm.load_img(filename)
	print("xml load time : %s" % (time.time() - start))
	
	sys.path.append("../view")
	from graphview import *
	
	v = GraphView(tm)
	v.mainloop()			
