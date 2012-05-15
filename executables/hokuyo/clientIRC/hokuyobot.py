#!/usr/bin/python3
# -*- coding: utf-8 -*-



import sys
import os
DIR_PATH = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join("..","..","clientIRC"))

from executablebot import ExecutableBot


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","clientIRC"))
EXEC = os.path.join(FILE_DIR,"..", "driver", "hokuyoApp.exe")
PROTOCOL_H = os.path.join(FILE_DIR,"..","..","..","protocole.h")

from executablebot import run


if __name__ == "__main__":
	run(
		nickname = "bridgebot",
		channel = "#hokuyo",
		exec_name = EXEC,
		protocol_file = PROTOCOL_H,
		protocol_prefixe = "QH_",
	)










