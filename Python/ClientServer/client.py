# -*- coding: utf-8 -*-

import socket

LISTEN_PORT = 4121

class Client(object):
	
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
		self.connect()
		# self.sock.settimeout(0.01)
		
	def connect(self):
		self.sock.connect(self.address)

	def sendall(self, data):
		if self.sock:
			self.sock.sendall(data)

	def recvall(self):
		if not self.sock:
			return None
		data = []
		BUFFER_SIZE = 1 << 16
		while True:
			rdata = self.sock.recv(BUFFER_SIZE)
			if not rdata:
				break
			data.append(rdata)
			if len(rdata) < BUFFER_SIZE:
				break
		return ''.join(data)

def test():
	client = Client('127.0.0.1', 4120)
	print client.address
	
	client.address = ('0.0.0.0', 4121)
	print client.address

if __name__ == '__main__':
	test()
