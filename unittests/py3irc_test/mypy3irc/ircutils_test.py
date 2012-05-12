


import unittest

from py3irc.mypyirc.ircutils import *



class IrcUtilsTestCase(unittest.TestCase):
	def test_canal_ircnormalize(self):
		self.assertEqual(canal_ircnormalize("coucou"), "#coucou")
		self.assertEqual(canal_ircnormalize("#coucou"), "#coucou")

	def test_canal_clientnormalize(self):
		self.assertEqual(canal_clientnormalize("coucou"), "coucou")
		self.assertEqual(canal_clientnormalize("#coucou"), "coucou")

	def test_channel_to_prefix_cmd(self):
		self.assertEqual(channel_to_prefix_cmd("coucou"), "cmd_coucou_")
		self.assertEqual(channel_to_prefix_cmd("#coucou"), "cmd_coucou_")

	def test_prefix_cmd_global(self):
		self.assertEqual(prefix_cmd_global(), "cmd__")

	def test_irc_cmd_to_func_name(self):
		self.assertEqual(irc_cmd_to_func_name("coucou", "f"), "cmd_coucou_f")
		self.assertEqual(irc_cmd_to_func_name("#coucou", "f"), "cmd_coucou_f")

	def test_irc_cmd_to_func_name_global(self):
		self.assertEqual(irc_cmd_to_func_name_global("f"), "cmd__f")
		self.assertEqual(irc_cmd_to_func_name_global("f"), "cmd__f")

	def test_func_name_to_irc_cmd(self):
		self.assertEqual(func_name_to_irc_cmd("cmd_debug_help"), "help")
		self.assertEqual(func_name_to_irc_cmd("cmd__help"), "help")

	def test_replace_channel_in_f_name(self):
		self.assertEqual(replace_channel_in_f_name("cmd_asserv_goto", "asservmini"), "cmd_asservmini_goto")
		self.assertEqual(replace_channel_in_f_name("cmd_asserv_goto", "#asservmini"), "cmd_asservmini_goto")

	def test_raw_msg_to_args_n_kwargs(self):
		self.assertEqual(raw_msg_to_args_n_kwargs("arg1 arg2 arg3 id_msg=56 kwarg1=hey"),
			(["arg1", "arg2","arg3"], {"id_msg":"56", "kwarg1":"hey"})
		)
		self.assertEqual(raw_msg_to_args_n_kwargs("arg1 arg2 arg3 kwarg1=hey"),
			(["arg1", "arg2","arg3"], {"id_msg":"42", "kwarg1":"hey"})
		)
		self.assertEqual(raw_msg_to_args_n_kwargs("arg1 arg2 arg3"),
			(["arg1", "arg2","arg3"], {"id_msg":"42"})
		)
		self.assertEqual(raw_msg_to_args_n_kwargs("id_msg=56 kwarg1=hey"),
			([], {"id_msg":"56", "kwarg1":"hey"})
		)


