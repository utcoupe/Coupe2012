


import unittest

import threading
import time


from py3irc.mypyirc.mypyircbot import *
from py3irc.mypyirc.ircdefine import SEP


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
		
		self.bot.wait_connection(10)
		self.assertTrue(self.bot.e_welcome.is_set())


		id_msg = 57
		a = 42
		self.bot._on_pubmsg("test", canal, "hello %s # id=%s" % (a, id_msg))

		time.sleep(1)

		self.assertEqual(str(executer.id_msg), str(id_msg))
		self.assertEqual(executer.a, a)

	def tearDown(self):
		self.bot.stop()
		



class MyPyIrcBotParseTestCase(unittest.TestCase):
	def setUp(self):
		self.str_protocole = """
			/**
				Un commentaire
				@param a
				@param b
			*/
			#define Q_PING			0
			
			/**
				Un commentaire
			*/
			#define Q2_PING2			42
		"""
		self.bot = MyPyIrcBot("localhost", 6667, "unittestbot", "unittest")

	def test_get_protocole(self):
		result = self.bot.get_protocole(self.str_protocole, "Q_")
		self.assertEqual(len(result), 1)
		cmd = result[0]
		self.assertEqual(cmd['id'], 0)
		self.assertEqual(cmd['name'], "PING")
		self.assertEqual(cmd['params'], ['a', 'b'])
		self.assertIsNotNone(cmd['doc'])

	def test_get_protocole_multi_prefixes(self):
		result = self.bot.get_protocole_multi_prefixes(self.str_protocole, ["Q_","Q2_"])
		self.assertEqual(len(result), 2)
		cmd = result["Q_"][0]
		self.assertEqual(cmd['id'], 0)
		self.assertEqual(cmd['name'], "PING")
		self.assertEqual(cmd['params'], ['a', 'b'])
		self.assertIsNotNone(cmd['doc'])
		cmd = result["Q2_"][0]
		self.assertEqual(cmd['id'], 42)
		self.assertEqual(cmd['name'], "PING2")
		self.assertEqual(cmd['params'], [])
		self.assertIsNotNone(cmd['doc'])
