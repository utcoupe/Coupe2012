

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","..","lib"))

from py3irc.mypyirc import MyPyIrcBot


class DebugBot(MyPyIrcBot):

	def cmd_testasserv_response(self, x, y, v, **kwargs):

if __name__ == "__main__":
	pass
