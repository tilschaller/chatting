.PHONY: all server client run 

all:
	${MAKE} server 
	${MAKE} client

server:
	gcc server/main.c -o chatting_server

client:
	gcc client/main.c -o chatting_client -lX11

run:
	./chatting_server &
	./chatting_client &
