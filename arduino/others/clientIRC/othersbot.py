#!/usr/bin/python3
# -*- coding:utf-8 -*-


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","clientIRC"))

import arduinobot

if __name__ == "__main__":
	arduinobot.run(
		nickname="othersbot",
		channel="other",
		serial_port="/dev/ttyACM0",
		protocole_file=os.path.join(FILE_DIR,"..","..","protocole.h"),
		protocole_prefixe="QO_")
