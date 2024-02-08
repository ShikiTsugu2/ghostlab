JC = javac
CC=gcc 

all:
	$(JC) -d . src/Client/Request.java src/Client/Client.java
	$(CC) -o server -pthread -I -Wall -g src/server/protocol_check.c src/server/player.c src/server/game.c src/server/list_player.c src/server/labyrinth.c src/server/list_game.c src/server/send.c src/server/server.c 
distclean:
	rm server *.class
