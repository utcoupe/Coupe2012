
import re
from .ircdefine import *

def canal_ircnormalize(canal):
	if not canal.startswith("#"):
		canal += "#"
	return canal.lower()

def canal_clientnormalize(canal):
	while canal.startswith('#'):
		canal = canal[1:]
	return canal.lower()

def channel_to_prefix_cmd(canal):
	"""
	>>> channel_to_prefix_cmd('debug')
	cmd_debug_
	
	@return le prefix associé à un canal
	"""
	canal = canal_clientnormalize(canal)
	return "cmd_%s_" % canal

def prefix_cmd_global():
	return channel_to_prefix_cmd("")

def irc_cmd_to_func_name(canal, irc_cmd):
	"""
	>>> irc_cmd_to_func_name('debug', 'help')
	cmd_debug_help
	
	@return la nom de la méthode associée au couple (canal, irc_cmd)
	"""
	return channel_to_prefix_cmd(canal) + irc_cmd.lower()

def irc_cmd_to_func_name_global(irc_cmd):
	"""
	>>> irc_cmd_to_func_name_global('help')
	cmd__irc_cmd
	
	@return le nom de la méthode globale (=disponibles sur tous les channels du bot) associée à irc_cmd
	"""
	return prefix_cmd_global() + irc_cmd.lower()

def func_name_to_irc_cmd(f_name):
	"""
	>>> func_name_to_irc_cmd('cmd_debug_help')
	help
	
	@return la commande irc associée à un nom de méthode
	"""
	return f_name.split('_',2)[2]

def replace_channel_in_f_name(f_name, new_channel):
	"""
	Remplace le channel dans le nom d'une méthode
	>>> replace_channel_in_f_name('cmd_asserv_goto', 'asservmini')
	cmd_asservmini_goto
	"""
	irc_cmd = func_name_to_irc_cmd(f_name)
	return irc_cmd_to_func_name(new_channel, irc_cmd)

def raw_msg_to_msg_n_options(raw_msg):
	"""
	
	"""
	raw_msg = raw_msg.strip().lower()
	if '#' in raw_msg:
		msg, str_options = raw_msg.split('#',1)
		msg.strip()
		str_options.strip()
	else:
		msg, str_options = raw_msg, ""
		
	options = {
		'id_msg': '42'
	}
	specs = {
		'id_msg': "id=(?P<id_msg>[-0-9]+)"
	}
	if str_options:
		for i,spec in specs.items():
			t = re.search(spec, str_options)
			if t:
				options[i] = t.group(i)

	return msg, options

	
def add_color(msg, *, color=None, background=None, bold=False):
		if color not in IRCCOLORS: color = None
		if background not in IRCCOLORS: background = None
		m = "{marker}{color},{background}{text}{marker}".format(
			marker=COLOR_MARKER,
			color=IRCCOLORS[color],
			background=IRCCOLORS[background],
			text=msg
		)
		if bold:
			m = BOLD_MARKER+m+BOLD_MARKER
		return m
