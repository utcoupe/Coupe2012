


import sys

class Logger:
	def __init__(self, file_path):
		self.terminal = sys.stdout
		self.log = open(file_path, 'a')

	def flush(self):
		self.terminal.flush()
		self.log.flush()
	
	def write(self, msg):
		self.terminal.write(msg)
		self.log.write(msg)
		self.flush()
