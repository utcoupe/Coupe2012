#!/usr/bin/python3
# -*- coding:utf-8 -*-


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","clientIRC"))

import arduinobot

if __name__ == "__main__":
	arduinobot.run(
		nickname="samplebot",
		channel="#test",
		serial_port="/dev/ttyACM0",
		protocol_file=os.path.join(FILE_DIR,"..","driver","protocole.h"),
		protocol_prefixe="Q_")
