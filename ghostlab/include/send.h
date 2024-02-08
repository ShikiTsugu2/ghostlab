#ifndef SEND_H 
#define SEND_H

#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include "list_game.h"
#include "protocol_check.h"

int send_list_game(int socket, struct  list_game * game, pthread_mutex_t lock);
int send_list_player(int socket, struct game * game, uint8_t m, pthread_mutex_t lock);
int register_to_aGame(int socket,struct list_game *games,char buff[50], pthread_mutex_t lock);
int send_size_labyrinth(int socket,struct list_game *games,char buff[50], pthread_mutex_t lock);
int create_newGame(int socket,struct list_game *games,char buff[50],uint8_t compteur, char *ip, char *port, pthread_mutex_t lock);
int send_unrok(int socket, int m);
int send_welco(int socket, game * game);
int send_posit(int socket, player *p);
int send_udp(char * ip, char * port, char * message);
int send_messa(char * ip, char * port, char *id, char * mess);
int send_messp(char * ip, char * port, char *id, char * mess);
int send_move(game* game, player* p, int socket, char* message);
int up_move(game *game, player * p, char *d);
int down_move(game *game, player * p, char *d);
int left_move(game *game, player * p, char *d);
int right_move(game *game, player * p, char *d);
int send_item(player * p, int socket, pthread_mutex_t lock);
int send_endgame(game* game);
int send_ittel(int socket, player *p,  game * g, char *message);
int send_killg(game* game, player* player, int socket);
int send_FGONE(char * ip, char *port, player *p);
int send_labsh(game* game, int socket);
int send_givch(int socket, player* sender, player* reciever);
int send_itbon(int socket, player * p);
int send_list_player_in_game(game* game,int socket);
int send_gobye(int socket, player * p, game* game);
int send_ittra(int socket, player *p, game * g, char *message);
int send_penal(char * ip, char *port, player *p);
int send_resize_labyrinth(int socket, game *game,char buff [50]);
#endif //SEND
