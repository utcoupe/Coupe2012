


import sys
TERMINAL = sys.stdout

class Logger:
	def __init__(self, file_path):
		self.terminal = TERMINAL
		self.log = open(file_path, 'a')

	def flush(self):
		self.terminal.flush()
	
	def write(self, msg):
		self.terminal.write(msg)
		self.terminal.flush()
		self.log.write(msg)
