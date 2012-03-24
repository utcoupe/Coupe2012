


import unittest

import threading
import time


from py3irc.mypyirc.mypyircbot import *


class DummyExecuter(Executer):
	def __init__(self):
		Executer.__init__(self)
		self.a = 0
		self.id_msg = ""
		
	def cmd__hello(self, a, **kwargs):
		""" Dummy ! """
		self.a = a
		self.id_msg = kwargs['id_msg']

class MyPyIrcBotConnTestCase(unittest.TestCase):
	def test_connection(self):
		canal = "#unittest"
		self.bot = MyPyIrcBot("localhost", 6667, "unittestbot", [canal])
		executer = DummyExecuter()
		self.bot.add_executer(executer)

		self.bot.cmd__hello(42, id_msg=45)
		
		t = threading.Thread(target=self.bot.start)
		t.setDaemon(True)
		t.start()
		
		self.bot.wait_connection(5)
		self.assertTrue(self.bot.e_welcome.is_set())


		id_msg = 57
		a = 42
		self.bot._on_pubmsg("unAuteurBidon", canal, ".hello %s id_msg=%s" % (a, id_msg))

		time.sleep(1)

		self.assertEqual(str(executer.id_msg), str(id_msg))
		self.assertEqual(executer.a, a)

	def tearDown(self):
		self.bot.stop()
		

