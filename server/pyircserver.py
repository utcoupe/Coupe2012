

import socketserver
import socket

import time, threading
import queue
import re
from collections import namedtuple
import datetime
import copy

from error_code import *
from logger import Logger


ParsedMsg = namedtuple('ParsedMsg', ['prefix', 'command', 'parameters'])
ParsedPrefix = namedtuple('ParsedPrefix', ['nick', 'name', 'domain'])
SEP = ' '
CRLF = '\n'
re_irc_str = '(:(?P<prefix>[!\w._@-]+){SEP})?(?P<command>[\d\w]+)({SEP}(?P<parameters>.+))?'.format(SEP=SEP, CRLF=CRLF)
re_irc = re.compile(re_irc_str)
re_prefix_str = '(?P<nick>[\w\d]+)!(?P<name>[\d\w]+)@(?P<domain>[\d\w_.-]+)'
re_prefix = re.compile(re_prefix_str)
def irc_parse(msg):
	t = re_irc.match(msg)
	if t:
		msgparsed = ParsedMsg(
			prefix = t.group('prefix') if t.group('prefix') else '',
			command = t.group('command').lower(),
			parameters = list(map(lambda x: x.strip(),parameters_parse(t.group('parameters'))))
		)
		return msgparsed
	else:
		return None
def irc_unparse(msg):
	return ':'+msg.prefix+SEP+str(msg.command).upper()+SEP+parameters_unparse(msg.parameters)+CRLF

def parameters_parse(s):
	if not s:
		return []
	split = s.split(':', 1)
	if len(split) > 1:
		short_params,long_param = split
	else:
		short_params,long_param = s,''
	l = short_params.split(SEP) + [long_param]
	l = list(filter(lambda x: x, l))
	return l
def parameters_unparse(params):
	if len(params) > 1:
		return SEP.join(params[:-1])+SEP+':'+params[-1]
	elif params:
		return ':'+params[-1]
	else:
		return ''

def prefix_parse(s):
	t = re_prefix.match(s)
	if t:
		return ParsedPrefix(
			nick = t.group('nick'),
			name = t.group('name'),
			domain = t.group('domain')
		)
def prefix_unparse(prefix):
	return prefix.nick+'!'+prefix.name+'@'+prefix.domain

def valide_nick(nick):
	return re.match('[\w\d]+', nick) != None
def valide_chan(chan):
	return re.match('#[\w\d]+', chan) != None

class Client(socketserver.StreamRequestHandler):
	
	def setup(self):
		super().setup()
		self.e_stop = threading.Event()
		self.to_send = queue.Queue()
		self.t_loop_recv = threading.Thread(target=self.loop_send)
		self.t_loop_send = threading.Thread(target=self.loop_recv)
		# timeout pour l'indentification
		self.request.settimeout(10)
		self.nick = ''
		self.realname = ''
		self.server.add_client(self)
		self.rooms = {}
	
	def stop(self):
		self.e_stop.set()
	
	def _stop(self):
		self.stop()
		self.server.remove_client(self)
	
	def is_auth(self):
		return self.nick != None
	
	@property
	def domain(self):
		return self.client_address[0]
	
	@property
	def prefix(self):
		return prefix_unparse(ParsedPrefix(nick=self.nick, name=self.nick, domain=self.domain))
		
	def send(self, msg):
		self.to_send.put(msg)
		
	def handle(self):
		self.t_loop_recv.start()
		self.t_loop_send.start()
		while not self.e_stop.is_set():
			self.e_stop.wait(1)
	
	def loop_recv(self):
		while not self.e_stop.is_set():
			try:
				self.data = self.rfile.readline().decode().strip()
			except Exception as ex:
				print(self.prefix, ex)
				self._stop()
			else:
				print("{} wrote:".format(self.prefix))
				print(self.data)
				if not self.data:
					self.send("are you alive ?") # test si ce n'est pas la connection qui s'est coupée
				self.server.process_incoming_msg(self, self.data)
	
	def loop_send(self):
		while not self.e_stop.is_set():
			try:
				msg = self.to_send.get(True, 1)
			except queue.Empty:
				pass
			else:
				try:
					self.wfile.write((msg.strip()+CRLF).encode())
				except Exception as ex:
					print(self.prefix, ex)
					self._stop()
	
	def validate_auth(self, nick):
		self.nick = nick
		self.request.settimeout(None)
			
		
		
class Room:
	def __init__(self, name):
		self.name = name
		self.clients = {}
		self.l_clients = threading.Lock()
	
	def add_client(self, client):
		self.l_clients.acquire()
		self.clients[client.nick] = client
		self.l_clients.release()
		client.rooms[self.name] = self
	
	def remove_client(self, client, msg=None):
		if not msg: msg = 'He is a pussy'
		self.l_clients.acquire()
		if client.nick in self.clients:
			del self.clients[client.nick]
		self.l_clients.release()
		self.send(irc_unparse(ParsedMsg(
			prefix = client.prefix,
			command = 'quit',
			parameters = (client.nick, msg)
		)))
	
	def get_clients(self):
		self.l_clients.acquire()
		d = copy.copy(self.clients)
		self.l_clients.release()
		return d
	
	def send(self, msg, *, exclude=set()):
		exclude = set(exclude)
		self.l_clients.acquire()
		for client in self.clients.values():
			if client not in exclude:
				client.send(msg)
		self.l_clients.release()


def need_params(n):
	"""
	decorator
	permet de préciser un nombre minimum de paramètres pour une fonction
	"""
	def call(f):
		def wrapped(self, client, msg):
			if len(msg.parameters) < n:
				client.send(self.make_response(ERR_NEEDMOREPARAMS, 'Not enough parameters'))
			else:
				f(self, client, msg)
		return wrapped
	return call

def need_auth(f):
	def wrapped(self, client, msg):
		if client.is_auth():
			f(self, client, msg)
		else:
			client.send(self.make_response(ERR_NOTREGISTERED, 'You have not registered'))
	return wrapped

class Server(socketserver.ThreadingMixIn, socketserver.TCPServer):
	def __init__(self, host_port):
		self.allow_reuse_address = True
		socketserver.TCPServer.__init__(self, host_port, Client)
		#self.daemon_threads = True
		self.rooms = {}
		self.l_rooms = threading.Lock()
		self.clients = {}
		self.l_clients = threading.Lock()
		self.date_launch = None
	
	def server_activate(self):
		super().server_activate()
	
	def server_close(self):
		super().server_close()
		self.l_clients.acquire()
		for client in self.clients.values():
			client.stop()
			self.shutdown_request(client.request)
			print("close %s" % client)
		self.l_clients.release()
		
	def start(self):
		try:
			self.date_launch = datetime.datetime.now()
			print("Listen on %s:%s..." % self.server_address)
			self.serve_forever()
		except KeyboardInterrupt:
			self.server_close()
			self.shutdown()
			print("Shutdown")
	
	def make_response(self, command, *args, prefix=None):
		if not prefix:
			prefix = str(self.server_address[0])
		if prefix.startswith(':'):
			prefix = prefix[1:]
		return irc_unparse(ParsedMsg(
			prefix = prefix,
			command = command,
			parameters = args
		))
	
	def nick_in_use(self, nick):
		return nick in self.clients
	
	def add_client(self, client):
		self.l_clients.acquire()
		if client.nick: # si il est déjà autentifié
			self.clients[client.nick] = client
		else:
			self.clients[id(client)] = client
		self.l_clients.release()
	
	def new_room(self, chan):
		self.l_rooms.acquire()
		self.rooms[chan] = Room(chan)
		self.l_rooms.release()
	
	def remove_client(self, client, msg=None):
		self.l_clients.acquire()
		if client.is_auth(): # si il est déjà autentifié
			if client.nick in self.clients:
				print("removed "+client.nick)
				del self.clients[client.nick] # on supprime l'ancienne clef
		else: # si il est nouveau
			if id(client) in self.clients:
				print("removed "+id(client))
				del self.clients[id(client)] # on le supprime des client non authentifiés
		self.l_clients.release()
		for room in client.rooms.values():
			room.remove_client(client, msg)
	
	def kill(self, client, msg=None):
		client.stop()
		self.remove_client(client, msg)
	
	def try_add_client(self, client, nick):
		err,msg = 0,''
		self.l_clients.acquire()
		if not self.nick_in_use(nick):
			if client.nick: # si il est déjà autentifié
				if client.nick in self.clients:
					del self.clients[client.nick] # on supprime l'ancienne clef
			else: # si il est nouveau
				if id(client) in self.clients:
					del self.clients[id(client)] # on le supprime des client non authentifiés
			self.clients[nick] = client
			client.nick = nick
		else:
			err,msg = ERR_NICKNAMEINUSE, 'nickname déjà utilisé'
		self.l_clients.release()
		client.validate_auth(nick)
		return err,msg
	
	def process_incoming_msg(self, client, msg_irc):
		if msg_irc:
			msg = irc_parse(msg_irc)
			if msg:
				method_name = '_cmd_'+msg.command 
				if method_name in dir(self):
					getattr(self, method_name)(client, msg)
				else:
					print("Err: commande inconnue : "+msg.command)
			else:
				print("Err: imposible de parser le message : "+msg_irc)
	
	@need_params(1)
	def _cmd_nick(self, client, msg):
		if not valide_nick(msg.parameters[0]):
			client.send(self.make_response(ERR_ERRONEUSNICKNAME, 'Invalid nickname parameters'))
		else:
			err,msg = self.try_add_client(client, msg.parameters[0])
			if err!=0:
				client.send(self.make_response(err,msg))
	
	@need_params(4)
	def _cmd_user(self, client, msg):
		client.realname = msg.parameters[3]
		client.send(self.make_response('001', client.nick, 'Welcome to the Python IRC Network '+client.prefix))
		client.send(self.make_response('002', client.nick, 'Your host is {host}, running version {version}'.format(host=self.server_address, version='PyIrcServer-0.0')))
		client.send(self.make_response('003', client.nick, 'This server was created %s' % self.date_launch))
		client.send(self.make_response('004', client.nick, '{host}'.format(host=self.server_address,)))
		client.send(self.make_response('005', client.nick))
		client.send(self.make_response(ERR_NOMOTD, client.nick, 'Message of the day file is missing.'))
		client.send(self.make_response(RPL_LUSERCLIENT, client.nick, 'There are %s users and %s services on %s servers'%(len(self.clients), 0, 1)))
		client.send(self.make_response(RPL_LUSERME, client.nick, 'I have %s clients and %s servers'%(len(self.clients), 1)))
		

	@need_auth
	def _cmd_quit(self, client, msg):
		if len(msg.parameters) > 0:
			self.kill(client, msg.parameters[0])
		else:
			self.kill(client)
	
	@need_params(1)
	@need_auth
	def _cmd_join(self, client, msg):
		canal = msg.parameters[0]
		if not valide_chan(canal):
			client.send(self.make_response(ERR_NOSUCHCHANNEL, client.nick, canal, 'Invalid channel name'))
		else:
			if canal not in self.rooms:
				self.new_room(canal)
			self.rooms[canal].add_client(client)
			self.rooms[canal].send(self.make_response('join', canal, prefix=client.prefix))
			self._cmd_names(client, ParsedMsg(prefix=msg.prefix, command='names', parameters=[canal]))
	
	@need_params(2)
	@need_auth
	def _cmd_privmsg(self, client, msg):
		nick_or_chan, privmsg = msg.parameters[0:2]
		response = self.make_response('privmsg', nick_or_chan, privmsg, prefix=client.prefix)
		if nick_or_chan in self.rooms:
			self.rooms[nick_or_chan].send(response, exclude=(client,))
		elif nick_or_chan in self.clients:
			self.clients[nick_or_chan].send(response)
		else:
			client.send(self.make_response(ERR_NORECIPIENT, client.nick, 'No such nick/channel'))
	
	@need_params(1)
	@need_auth
	def _cmd_names(self, client, msg):
		chan = msg.parameters[0]
		if chan in self.rooms:
			clients = self.rooms[chan].get_clients()
			nicks = map(lambda c: c.nick, clients.values())
			nicks_str = SEP.join(nicks)
			client.send(self.make_response(RPL_NAMREPLY, client.nick, '=', chan, nicks_str))
			client.send(self.make_response(RPL_ENDOFNAMES, client.nick, chan, 'End of /NAMES list'))
		
	def __repr__(self):
		return '<PyIrcServer(%s,%s)' % self.server_address


if __name__=='__main__':
	import sys
	import os
	HOST, PORT = '', int(sys.argv[1])
	LOG_FILE = os.path.abspath(__file__)+'.log'
	sys.stdout = Logger(LOG_FILE)
	server = Server((HOST,PORT))
	server.start()
	#msgparsed = irc_parse('JOIN #test coucou\n')
	msgparsed = irc_parse('nick tom')
	print(msgparsed)
	if msgparsed.prefix:
		print(prefix_parse(msgparsed.prefix))
