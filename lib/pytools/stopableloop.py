
class StopableLoop:
	def __init__(self, func, sleep):
		self.func = func
		self.sleep = sleep
		self.event = threading.Event()
		self.event.clear()

	def start(self):
		if self.event.is_set():
			print("la boucle est déjà lancée !")
		else:
			while not self.event.is_set():
				self.func()
				self.event.wait(self.sleep)

	def stop(self):
		self.event.set()
		
