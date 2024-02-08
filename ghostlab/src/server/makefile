CC=gcc #Le compilateur à utiliser
TARGET=server #Nom du fichier à créer

all:
	$(CC) -o $(TARGET) -pthread -I -Wall -g list_player.c list_game.c send.c server.c
distclean:
	rm $(TARGET)
	rm test
