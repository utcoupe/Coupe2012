#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
sys.path.append(os.path.join("..","lib","py3irc"))

import irclib
import ircbot
from mypyircbot import *

import threading
import time

from define import *


_COMPILED_F_ = None


class SimuIrcBot(MyPyIrcBot):
	def __init__(self, server_ip, server_port, channels):
		MyPyIrcBot.__init__(self,
			server_ip, server_port,
			"simubot",
			channels
		)

		self.robots = {}
		self.running = True
		self.t = threading.Thread(None, self.loop_send, "simubot-loopsend")
		self.t.start()

	def add_robot(self, i, robot):
		self.robots[i] = robot
		for f_name in filter(lambda s: s.startswith("cmd_"), dir(robot)):
			f = getattr(robot, f_name)
			f_args = inspect.getargspec(f).args
			if len(f_args) > 0 and 'self' == f_args[0]:
				f_params = f_args+['**kwargs']
			else:
				f_params = ['self']+f_args+['**kwargs']
			if len(f_args) > 0 and 'self' == f_args[0]:
				robot_args = f_args[1:]
			else:
				robot_args == f_args
			robot_args = list(map(lambda s: 'int(%s)' %s, robot_args))
			robot_args.append('**kwargs')
			d = {}
			code = "def f(%s):\n" % ','.join(f_params)
			code += "	self.robots[{i}].{f_name}({args})\n".format(f_name=f_name, i=i, args=','.join(robot_args))
			code += "	return '%s'" % f_name
			exec(code, d)
			d['f'].__doc__ = f.__doc__
			setattr(self, f_name, d['f'])

	def stop(self):
		self.running = False
		if self.serv:
			self.serv.disconnect("Tchuss")

	def loop_send(self):
		while self.running:
			for r in self.robots.values():
				canal, msg = r.get_msg()
				while canal:
					self.send(canal,msg)
					canal, msg = r.get_msg()
			time.sleep(0.01)
				
	
