

from node import *


class NodeList:
	def __init__(self, begin):
		self.begin = begin
		begin.in_open_list = True

	def is_empty(self):
		return not self.begin
	
	def pop_min_score(self):
		if self.begin:
			node = self.begin
			mini = node.score
			while node:
				mini = min(node.score,mini)
				node = node.next_in_open
			node = self.begin
			if self.begin.score == mini:
				self.begin = self.begin.next_in_open
				node.in_open_list = False
			else:
				while node:
					if node.score == mini:
						node.previous_in_open.next_in_open = node.next_in_open
						if node.next_in_open:
							node.next_in_open.previous_in_open = node.previous_in_open
						node.in_open_list = False
						break
					node = node.next_in_open
			return node
		else:
			return None


	def push_uniq(self, node):
		if not node.in_open_list:
			if not self.is_empty():
				self.begin.previous_in_open = node
			node.next_in_open = self.begin
			self.begin = node
			node.in_open_list = True

	def __len__(self):
		node = self.begin
		i = 0
		while node:
			i+=1
			node = node.next_in_open
		return i
		
	def __str__(self):
		node = self.begin
		s = "<NodeList\n"
		while node:
			s += str(node) + '\n'
			node = node.next_in_open
		s += (">")
		return s
			


import doctest
doctest.testfile("doctest/nodelist.txt")
