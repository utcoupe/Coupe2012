




def load_func(d):
	cmd = """
import glob
import os
import doctest


def massive_exec(dir_path, suffix=""):
	if glob.glob(dir_path+suffix+"/*"):
		test_files = glob.glob(dir_path+suffix+"/*.test.py")
		for f in test_files:
			print (f)
			if os.path.isfile(f):
				doctest.testfile(f)
		massive_exec(dir_path, suffix+"/*")

	"""
	exec(cmd, d)
