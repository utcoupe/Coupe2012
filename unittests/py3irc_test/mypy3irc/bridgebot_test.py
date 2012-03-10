
import unittest

import threading
import time


from py3irc.mypyirc.bridgebot import *
from py3irc.mypyirc.ircdefine import SEP


class MyPyIrcBotConnTestCase(unittest.TestCase):
	def test_connection(self):
		canal = "#unittest"
		self.bot = MyPyIrcBot("localhost", 6667, "unittestbot", [canal])
		
		t = threading.Thread(target=self.bot.start)
		t.setDaemon(True)
		t.start()
		
		self.bot.wait_connection(5)
		self.assertTrue(self.bot.e_welcome.is_set())

	def tearDown(self):
		self.bot.stop()
		



class MyPyIrcBotParseTestCase(unittest.TestCase):
	def setUp(self):
		self.bot = BridgeBot("localhost", 6667, "unittestbot", "unittest", None, None)

	def test_get_protocol(self):
		str_protocol = """
			#define   SEP		'#'
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
		sep,commands = self.bot.get_protocol(str_protocol, "Q_")
		self.assertEqual(sep,'#')
		self.assertEqual(len(commands), 1)
		cmd = commands[0]
		self.assertEqual(cmd['id'], 0)
		self.assertEqual(cmd['name'], "PING")
		self.assertEqual(cmd['params'], ['a', 'b'])
		self.assertIsNotNone(cmd['doc'])

	def test_get_protocol2(self):
		str_protocol = ""
		self.assertRaises(ProtocolException, self.bot.get_protocol, str_protocol, "Q_")
		

