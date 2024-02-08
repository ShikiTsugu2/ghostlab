#ifndef GAME_H
#define GAME_H

#include <arpa/inet.h>
#include "list_player.h"
#include "labyrinth.h"
#include <unistd.h>
#include <pthread.h>
typedef struct game game;
struct game
{
    uint8_t num;
    uint8_t nb_player;
    list_player *players;
    labyrinth *labyrinth;
    char ip[16];
    char port[5];
    pthread_mutex_t lock;
    int items;
    int pipes[2];
    //Ajouter une liste de joueur
};
static const uint16_t DEFAULT_H = 10;
static const uint16_t DEFAULT_W = 10;
static const uint8_t DEFAULT_F = 10;
struct game * create_game(uint8_t num, player * p, char *ip, char * port );
void free_game(game * g);
#endif // GAME

