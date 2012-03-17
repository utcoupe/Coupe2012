#!/usr/bin/python3
# -*- coding: utf-8 -*-

import re
import glob
import os



re_file = re.compile('^.+?[.](cpp|h|hpp|ino|pde)$')
re_arduino_ver = re.compile('arduino-[\d.]+.?(?=[\'"])')
re_include = re.compile('^\s*#include\s+[\'"](?P<include>.+?)[\'"].*$')
re_include = re.compile('#include\s+[\'"](?P<include>(?![\'"]).+)')


def get_ifiles(top, forbids):
	re_forbid = re.compile('|'.join(map(lambda x: '('+x+')', forbids)))
	for x in os.walk(top):
		dirpath, dirnames, filenames = x
		if not re_forbid.search(dirpath):
			for fname in filenames:
				if fname == "SConstruct" or re_file.match(fname):
					full_file_path = os.path.join(dirpath, fname)
					yield full_file_path


def remove_duplicate_includes(s):
	ss = ""
	includes = set()
	for line in s.split('\n'):
		t = re_include.search(line)
		if t:
			include = t.group("include")
			if include not in includes:
				includes.add(t.group("include"))
				ss += line+"\n"
		else:
			ss += line+"\n"

	if ss.endswith("\n\n"):
		ss = ss[:-1]
	
	return ss

def process_file(full_file_path, old, new):
	
	print(full_file_path)

	# lecture
	f = open(full_file_path)
	s = f.read()
	f.close()

	
	must_rewrite = False # variable indiquant si une modification doit être effectuée
	
	# remplacement
	if full_file_path.endswith('SConstruct'):
		for t in re_arduino_ver.findall(s):
			if t != new.libname:
				print(t,"->", new.libname)
		s = re_arduino_ver.sub(new.libname, s)
		must_rewrite = True
	elif old.re_include.search(s):
		s = old.re_include.sub(new.include, s)
		s = remove_duplicate_includes(s)
		must_rewrite = True

	if must_rewrite:
		# écriture
		f = open(full_file_path, 'w')
		f.write(s)
		f.close()

		# éventuellement renommage
		froot, fext = os.path.splitext(full_file_path)
		if fext == old.extension:
			new_path = froot+new.extension
			print("full_file_path -> "+new_path)
			os.rename(full_file_path, new_path)

class VersionData:
	def __init__(self, *, extension, include, re_include, libname):
		self.extension = extension if extension.startswith(os.extsep) else os.extsep+extension
		self.include = include
		self.re_include = re_include
		self.libname = libname


if __name__ == "__main__":
	import sys
	
	old = '0'
	new = '1'

	if len(sys.argv) > 1:
		old, new = new, old
	
	top = 'arduino/'

	forbids = [ 'build' , '__pycache__', 'arduino-.*']

	versions = {
		'0': VersionData(
			extension 		= '.pde',
			include			= '#include "WProgram.h"\n#include "wiring.h"',
			re_include		= re.compile('(#include\s+.wiring[.]h.)|(#include\s+.WProgram[.]h.)'),
			libname			= 'arduino-0022/',
		),
		'1': VersionData(
			extension 		= '.ino',
			include			= '#include "Arduino.h"\n',
			re_include		= re.compile('#include\s+.Arduino[.]h.'),
			libname			= 'arduino-1.0/',
		)
	}

	for fpath in get_ifiles(top, forbids):
		process_file(fpath, versions[old], versions[new])





