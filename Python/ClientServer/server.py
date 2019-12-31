# -*- coding: utf-8 -*-

import socket
import select
import time
import struct

LISTEN_PORT = 4121

class Server(object):
	
	def __init__(self, ip_addr, port, cid=None):
		self.cid = cid
		self.ip_addr = ip_addr
		self.port = port
		self.sock = None

	@property
	def address(self):
		return (self.ip_addr, self.port)

	@address.setter
	def address(self, address):
		self.ip_addr, self.port = address

	def init_net(self):
		# Socket Type: https://docs.oracle.com/cd/E19455-01/806-1017/sockets-4/index.html
		# Address Family: https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_73/rzab6/address.htm
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		# self.sock.settimeout(2)
		self.bind()
		
	def bind(self):
		self.sock.bind(self.address)

	def listen(self):
		rlist, wlist, xlist = [], [], []
		rlist.append(self.sock)
		self.sock.listen(10)
		print('start listen .......')
		while True:
			rl, wl, error = select.select(rlist, wlist, xlist, 5)
			if rl and wl and error:
				print('current select: {} {} {}'.format(rl, wl, error))
			for s in rl:
				if s == self.sock:
					conn, addr = self.sock.accept()
					print('Connected by ', addr)
					rlist.append(conn)
				else:
					try:
						data = self.recvall(s)
						if data:
							print('recv %s byte(s) do echo !!!!' % len(data))
						else:
							print('sync timestamp !!!')						
							data = struct.pack('i', time.time())
						self.sendall(s, data)
					except socket.error:
						print('Remove sock {} for send/recv errors!!!'.format(s))
						if s in rlist:
							rlist.remove(s)
					
	def sendall(self, sock, data):
		if sock:
			sock.sendall(data)

	def close(self, sock=None):
		if not sock:
			sock = self.sock
		if sock:
			sock.close()
			
	def recvall(self, sock):
		if not sock:
			return None
		data = []
		BUFFER_SIZE = 1 << 16
		while True:
			rdata = sock.recv(BUFFER_SIZE)
			if not rdata:
				break
			data.append(rdata)
			if len(rdata) < BUFFER_SIZE:
				break
		return ''.join(data)

def test():
	server = Server('127.0.0.1', 4120)
	print server.address
	
	server.address = ('0.0.0.0', 4121)
	print server.address

if __name__ == '__main__':
	test()
