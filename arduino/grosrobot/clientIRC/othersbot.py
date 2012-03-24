#!/usr/bin/python3
# -*- coding:utf-8 -*-


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","clientIRC"))

import arduinobot

if __name__ == "__main__":
	arduinobot.run(
		nickname="others",				# nom du bot
		channel="#others",					# canal sur lequel se connecter
		serial_port="/dev/ttyACM0",			# port de l'arduino
		protocol_file=os.path.join(FILE_DIR,"..","..","protocole.h"),	# où trouver le protocole.h
		protocol_prefixe="QO_")				# préfix des commandes dans le protocole
