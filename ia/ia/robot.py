



import copy
import time


from geometry import Segment, Vec




class Robot:
	def __init__(self, p, ng):
		self.init_actions = []
		self.init_pos = p
		self.ng = ng
		self.asserv = None
		self.reset()
		self.path_was_intersected = False

	def set_actions(self, actions):
		self.init_actions = actions
		self.actions = copy.copy(actions)

	def set_asserv(self, asserv):
		"""
		Link le client asservissement
		"""
		self.asserv = asserv

	def set_visio(self, visio):
		self.visio = visio
	
	def set_extras(self, extras):
		"""
		Link le client extras (téléportation)
		"""
		self.extras = extras

	def set_actionneurs(self, actionneurs):
		self.actionneurs = actionneurs

	def reset(self):
		self.pos = Vec(self.init_pos)
		self.a = 0
		self.time_pos_updated = 0
		self.in_action = False
		self.path = []
		self.target_action = None

		self.actions = copy.copy(self.init_actions)
		

	def reset_target_action(self):
		self.path = []
		self.target_action = None
	
	def set_new_path(self, path):
		self.path = path
		self.current_goal = path[0]

	def set_target_action(self, action, path):
		self.reset_target_action()
		self.target_action = action
		self.set_new_path(path)

	def reach_next_checkpoint(self):
		return (self.pos - self.path[0]).norm2() <= 100

	def get_next_checkpoint(self):
		self.path.pop(0)
		if self.path:
			return self.path[0]
		else:
			return None

	def is_arrive(self):
		return len(self.path) == 0

	def is_new_action(self, action):
		return action != self.target_action

	def update_pos(self, p):
		self.pos = Vec(p)
		self.time_pos_updated = time.time()

	def is_path_intersected(self):
		for poly in self.ng.dynamic_obstacles:
			if self.poly_intersect_path(poly):
				return True
		return False
	
	def poly_intersect_path(self, poly):
		last = self.pos
		for current in self.path:
			s = Segment(last, current)
			if poly.intersect(s):
				return True
		return False

	def __repr__(self):
		return "Robot(pos=%s, a=%s)" % (self.pos, self.a)



