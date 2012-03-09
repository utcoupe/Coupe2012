


import unittest

import threading
import time


from py3irc.mypyirc.mypyircbot import *


class DummyExecuter(Executer):
	def cmd_unittest_hello(self, a, **kwargs):
		""" Dummy ! """
		self.a = a
		self.id_msg = kwargs['id_msg']

class MyPyIrcBotConnTestCase(unittest.TestCase):
	def test_connection(self):
		canal = "#unittest"
		self.bot = MyPyIrcBot("localhost", 6667, "unittestbot", [canal])
		executer = DummyExecuter()
		self.bot.add_executer(executer)
		
		t = threading.Thread(target=self.bot.start)
		t.setDaemon(True)
		t.start()
		
		self.bot.wait_connection(5)
		self.assertTrue(self.bot.e_welcome.is_set())


		id_msg = 57
		a = 42
		self.bot._on_pubmsg("test", canal, ".hello %s # id=%s" % (a, id_msg))

		time.sleep(1)

		self.assertEqual(str(executer.id_msg), str(id_msg))
		self.assertEqual(executer.a, a)

	def tearDown(self):
		self.bot.stop()
		

