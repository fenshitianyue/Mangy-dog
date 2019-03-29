CC=g++
CFLAGS=-std=c++11

all:clientMain.cc serverMain.cc server.o client.o
	$(CC) $(CFLAGS) serverMain.cc server.o -o chatroom_server
	$(CC) $(CFLAGS) clientMain.cc client.o -o chatroom_client

server.o:server.cc server.h common.h
	$(CC) $(CFLAGS) -c server.cc

client.o:client.cc client.h common.h
	$(CC) $(CFLAGS) -c client.cc

clean:
	rm -f *.o chatroom_client chatroom_server
