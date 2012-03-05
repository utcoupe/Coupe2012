#!/usr/bin/python3
# -*- coding: utf-8 -*-



import unittest
import sys

testloader = unittest.TestLoader()

for testsuite in testloader.discover("ia_test", pattern="*_test.py"):
	unittest.TextTestRunner(verbosity=1).run(testsuite)



