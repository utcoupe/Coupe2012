



def canal_ircnormalize(canal):
	if not canal.startswith("#"):
		canal += "#"
	return canal.lower()

def canal_clientnormalize(canal):
	while canal.startswith('#'):
		canal = canal[1:]
	return canal.lower()

def channel_to_prefix_cmd(canal):
	canal = canal_clientnormalize(canal)
	return "cmd_%s_" % canal

def irc_cmd_to_func_name(canal, irc_cmd):
	canal = canal_clientnormalize(canal)
	return channel_to_prefix_cmd(canal) + irc_cmd.lower()

def func_name_to_irc_cmd(f_name):
	return f_name.split('_',2)[2]

def replace_channel_in_f_name(f_name, new_channel):
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
