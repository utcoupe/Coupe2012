#!/usr/bin/python3
# -*- coding: utf-8 -*-



import unittest
import sys

testloader = unittest.TestLoader()

for testsuite in testloader.discover("geometry_test", pattern="*_test.py"):
	unittest.TextTestRunner(verbosity=1).run(testsuite)

for testsuite in testloader.discover("py3irc_test", pattern="*_test.py"):
	unittest.TextTestRunner(verbosity=1).run(testsuite)

"""
if __name__ == "__main__":

	from massivetests import load_func

	load_func(locals())
	massive_exec("doctests")

"""


