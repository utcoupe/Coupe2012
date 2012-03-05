#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","lib"))


from py3irc.mypyirc import mypyircbot
from py3irc.mypyirc.ircdefine import *

import subprocess
import glob
import re


class MonitoringBot(mypyircbot.MyPyIrcBot):
	def __init__(self, server_ip, server_port):
		self.canal = "#monitoring"
		mypyircbot.MyPyIrcBot.__init__(self, server_ip, server_port, "monitoringbot", [self.canal])
		self.load_all_bash()

	def load_all_bash(self):
		"""
		Récupérer tous les exec bash et lancer load_bash(..)
		"""
		for path in glob.iglob(os.path.join(FILE_DIR,"*.sh")):
			print("file :",path)
			self.load_bash(path)
			
	def load_bash(self, path):
		"""
		Récupérer les informations dans le bash et créer la fonction "cmd_*" qui va avec.
		@param path chemin vers l'exec bash
		"""
		re_name = re.compile("([^%s]+%s)+(?P<filename>[^.]*).*" % (os.sep,os.sep))
		spec_params = '@param\s+(?P<param>[a-zA-Z_]\w*)'
		re_params = re.compile(spec_params)
		t = re_name.search(path)
		if t:
			filename = t.group("filename")
			f = open(path)
			params = [p.group("param") for p in re_params.finditer(f.read())]
			self.add_function(path, filename, params)
		else:
			print("ERROR : can't get filename")
		
	def add_function(self, path, cmd_name, params):
		"""
		Ajouter une commande à la classe en cours.
		@param path le chemin vers l'exec bash
		@param cmd_name le nom de la commande
		@param params liste des paramètres
		"""
		global _RESULT_EXEC_
		print("Generate function cmd_%s(%s)" % (cmd_name,params))
		code = "def _RESULT_EXEC_(%s):\n" % ','.join(['self']+params+['**kwargs'])
		code += "	return MonitoringBot._MonitoringBot__cmd('{path}',{params})".format(path=path, params=','.join(params))
		exec(code, globals())
		_RESULT_EXEC_.__doc__ = "doc not available for the moment"
		self.add_cmd_function(self.canal, cmd_name, _RESULT_EXEC_)

	@staticmethod
	def __cmd(exec_file, *params):
		"""
		Fonction interne permettant de lancer un script bash et de récupérer le retour
		@param exec_file chemin vers l'exec bash
		@param params les paramètres à envoyer à l'executable
		"""
		print(params)
		p = subprocess.Popen([exec_file]+list(params), stdout=subprocess.PIPE)
		msg = str(p.communicate()[0], "utf-8").strip()
		return msg if msg else "-1"

	def write_rep(self, msg, id_msg=42):
		""" Surcharge de la classe parente """
		self.send(self.canal, SEP.join([str(id_msg), str(msg)]))


if __name__ == "__main__":
	bot = MonitoringBot("localhost", 6667)
	bot.start()
