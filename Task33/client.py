#!/usr/bin/python3

import socket

#Create socket object
clientsocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

#host = '192.168.1.104'
host = socket.gethostname()

port = 4440

clientsocket.connect(('127.0.0.1', port)) #You can substitue the host with the>

#Receiving a maximum of 1024 bytes
message = clientsocket.recv(1024)

clientsocket.close()

print(message.decode('ascii'))
