#!/usr/bin/python3
# -*- coding: utf-8 -*-

if __name__ == "__main__":
	import sys
	import os
	sys.path.append(os.path.join(os.path.dirname(__file__), "..", "lib"))

	from massivetests import load_func

	massive_exec("doctests")


