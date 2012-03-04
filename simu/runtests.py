#!/usr/bin/python3
# -*- coding: utf-8 -*-


if __name__ == "__main__":
	import glob
	import os
	import doctest


	def execute_and_rec(dir_path, suffix="*"):
		files = glob.glob(dir_path+suffix)
		if files:
			for f in files:
				if os.path.isfile(f):
					doctest.testfile(f)
			execute_and_rec(dir_path, suffix+"/*")

	execute_and_rec("doctests/")
	




