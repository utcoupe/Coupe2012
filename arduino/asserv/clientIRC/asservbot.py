# -*- coding: utf-8 -*-

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","clientIRC"))

import arduinobot
SEP = '.'

class AsservBot(arduinobot.ArduinoBot):
	def __init__(self, server_ip, server_port, serial_port):
		arduinobot.ArduinoBot.__init__(self, server_ip, server_port, "asservbot", "#asserv", serial_port, 115200)

	def cmd_id(self):
		""" le nom du client : 'asserv' """
		return "0"

	def cmd_ping(self):
		""" pong """
		return "1"

	def cmd_goto(self, x, y, v):
		""" Aller à un point (x,y) de la carte à une vitesse (v). absolut """
		return SEP.join(["2", x, y, v])

	def cmd_gotor(self, x, y, v):
		""" Aller à un point (x,y) de la carte à une vitesse (v). relatif """
		return SEP.join(["3", x, y, v])
	
	def cmd_turn(self, a, v):
		""" tourner """
		return SEP.join(["4", a, v])

	def cmd_turnr(self, a, v):
		""" tourner """
		return SEP.join(["5", a, v])

	def cmd_pos(self):
		""" position """
		return "6"

	def cmd_recal(self, c):
		""" recalage """
		return "7" + SEP + c

	def cmd_cancel(self):
		""" arreter le robot et vider la FIFO """
		return "13"

	def cmd_stop(self):
		""" arrete le robot (sans vider la fifo, 'resume' pour repartir """
		return "14"

	def cmd_resume(self):
		""" relancer le robot après un stop """
		return "15"


if __name__ == "__main__":

	AsservBot("10.42.43.94",6667,"/dev/ttyACM1").start()
	
