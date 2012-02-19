#!/usr/bin/python3
# -*- coding: utf-8 -*-


import time

print("hello")

while True:
	try:
		m = input()
	except Exception as ex:
		print(ex)
	else:
		print(m)
	time.sleep(0.01)
