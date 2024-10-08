.PHONY: all server client run 

all:
	${MAKE} server 
	${MAKE} client

server:
	gcc server/main.c -o serverb

client:
	gcc client/main.c -o clientb -lX11 -lGL

run:
	./serverb &
	./clientb &
