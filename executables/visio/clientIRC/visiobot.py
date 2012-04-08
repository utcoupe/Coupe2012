#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","clientIRC","lib"))
EXEC = os.path.join(FILE_DIR,"..","driver","bin","Debug", "camera")
PROTOCOL_H = os.path.join(FILE_DIR,"..","..","..","protocol.h")

from executablebot import run


if __name__ == "__main__":
	run({
		"nickname": "visobot",
		"channel": "visio",
		"exec_name": EXEC,
		"protocol_file": PROTOCOL_H,
		"protocol_prefixe": "QV_",
	})
