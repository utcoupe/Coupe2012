import socketserver
import socket

import time, threading
import queue
import re
from collections import namedtuple
import datetime




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
			parameters = parameters_parse(t.group('parameters'))
		)
		return msgparsed
	else:
		return None
def irc_unparse(msg):
	return ':'+msg.prefix+SEP+str(msg.command)+SEP+parameters_unparse(msg.parameters)+CRLF

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
	if params and ' ' in params[-1]:
		return SEP.join(params[:-1])+SEP+':'+params[-1]
	else:
		return SEP.join(params)

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
	
	def stop(self):
		self.e_stop.set()
	
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
				print(ex)
				self.stop()
				self.server.remove_client(self)
			else:
				print("{} wrote:".format(self.prefix))
				print(self.data)
				self.server.process_incoming_msg(self, self.data)
	
	def loop_send(self):
		while not self.e_stop.is_set():
			try:
				msg = self.to_send.get(True, 1)
			except queue.Empty:
				pass
			else:
				self.wfile.write((msg.strip()+CRLF).encode())
	
	def validate_auth(self, nick):
		self.nick = nick
		self.request.settimeout(None)
			
		
		
class Room:
	def __init__(self, name):
		self.name = name
		self.clients = {}

	def send_msg(self, msg):
		for client in self.clients:
			client.send_msg(msg)

	

class Server(socketserver.ThreadingMixIn, socketserver.TCPServer):
	def __init__(self, host_port):
		socketserver.TCPServer.__init__(self, host_port, Client)
		self.daemon_threads = True
		self.allow_reuse_address = True
		self.rooms = {}
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
	
	def make_response(self, command, *args):
		return irc_unparse(ParsedMsg(
			prefix = ':'+str(self.server_address),
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
	
	def remove_client(self, client):
		self.l_clients.acquire()
		if client.nick: # si il est déjà autentifié
			if client.nick in self.clients:
				del self.clients[client.nick] # on supprime l'ancienne clef
		else: # si il est nouveau
			if id(client) in self.clients:
				del self.clients[id(client)] # on le supprime des client non authentifiés
		self.l_clients.release()
	
	def kill(self, client):
		self.remove_client()
		client.stop()
	
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
		msg = irc_parse(msg_irc)
		if msg:
			method_name = '_cmd_'+msg.command 
			if method_name in dir(self):
				getattr(self, method_name)(client, msg)
			else:
				print("Err: commande inconnue : "+msg.command)
		else:
			print("Err: imposible de parser le message : "+msg_irc)
	
	def _cmd_nick(self, client, msg):
		if not msg.parameters:
			client.send(self.make_response(ERR_NONICKNAMEGIVEN, 'No nickname given'))
		elif not valide_nick(msg.parameters[0]):
			client.send(self.make_response(ERR_ERRONEUSNICKNAME, 'Invalid nickname parameters'))
		else:
			err,msg = self.try_add_client(client, msg.parameters[0])
			if err!=0:
				client.send(self.make_response(err,msg))
	
	def _cmd_user(self, client, msg):
		if len(msg.parameters) < 4:
			client.send(self.server.make_response(ERR_NEEDMOREPARAMS, 'Not enough parameters'))
		else:
			client.realname = msg.parameters[3]
			client.send(self.make_response('001', client.nick, 'Welcome to the Python IRC Network '+client.prefix))
			client.send(self.make_response('002', client.nick, 'Your host is {host}, running version {version}'.format(host=self.server_address, version='PyIrcServer-0.0')))
			client.send(self.make_response('003', client.nick, 'This server was created %s' % self.date_launch))
			client.send(self.make_response('004', client.nick, '{host}'.format(host=self.server_address,)))
			client.send(self.make_response('005', client.nick))
	
	def _cmd_quit(self, client, msg):
		self.kill(client)
	
	def __repr__(self):
		return '<PyIrcServer(%s,%s)' % self.server_address

if __name__=='__main__':
	import sys
	HOST, PORT = 'localhost', int(sys.argv[1])
	server = Server((HOST,PORT))
	server.start()
	#msgparsed = irc_parse('JOIN #test coucou\n')
	msgparsed = irc_parse('nick tom')
	print(msgparsed)
	if msgparsed.prefix:
		print(prefix_parse(msgparsed.prefix))
