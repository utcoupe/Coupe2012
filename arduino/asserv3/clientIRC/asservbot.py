#!/usr/bin/python3
# -*- coding:utf-8 -*-


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","clientIRC"))

import arduinobot

import matplotlib
matplotlib.use('Qt4Agg')
from pylab import *



class DebugBot(arduinobot.ArduinoBot):
	def __init__(self, server_ip, server_port, nickname, channel, serial_port, serial_baudrate, protocol_file, protocol_prefixe):
		arduinobot.ArduinoBot.__init__(self, server_ip, server_port, nickname, channel, serial_port, serial_baudrate, protocol_file, protocol_prefixe)
		self.t = []
		self.x = []
		self.v = []
		self.pwm = []

	def send_response(self, canal, id_msg, msg):
		arduinobot.ArduinoBot.send_response(self, canal, id_msg, msg)

		if msg.startswith("plot"):
			msg_split = msg.split()
			t = float(msg_split[1].strip())
			x = float(msg_split[2].strip())
			v = float(msg_split[3].strip())
			pwm = float(msg_split[4].strip())
			self.t.append(t/1000)
			self.x.append(x)
			self.v.append(v)
			self.pwm.append(pwm)

	def cmd_testasserv_show(self, **kwargs):
		hold(True)
		"""t = arange(0.0, 2.0, 0.01)
		s = sin(2*pi*t)
		c = cos(2*pi*t)
		plot(t, s, linewidth=1.0)
		plot(t, c, linewidth=1.0)

		xlabel('time (s)')
		ylabel('voltage (mV)')
		title('About as simple as it gets, folks')
		"""
		plot(self.t, self.x, linewidth=1.0)
		plot(self.t, self.v, linewidth=1.0)
		plot(self.t, self.pwm, linewidth=1.0)
		
		xlabel('time (s)')
		ylabel('')
		title('title')
		grid(True)
		show()

	def cmd_testasserv_clean(self, **kwargs):
		self.t = []
		self.x = []
		self.v = []
		self.pwm = []
		
if __name__ == "__main__":
	arduinobot.run(
		bot_class=DebugBot,
		nickname="asservbot",
		channel="#testasserv",
		serial_port="/dev/ttyACM0",
		protocol_file=os.path.join(FILE_DIR,"..","..","protocole.h"),
		protocol_prefixe="QA_")
