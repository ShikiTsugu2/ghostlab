
#ifndef LABYRINTH_H
#define LABYRINTH_H

#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include <string.h>


typedef struct labyrinth labyrinth;
struct labyrinth
{
    uint16_t h;
    uint16_t w;
    uint8_t nb_ghosts;
    char **laby;
};
typedef struct cellule cellule;
struct cellule
{
    uint16_t x;
    uint16_t y;
   
};
static const char PLAYER = 'P';
static const char GHOST = 'G';
static const char WALL = '#';
static const char ITEM = 'O';
static const char TRAP = 'X';
static const char EMPTY = ' ';

labyrinth * create_labyrinth(uint16_t h, u_int16_t w, uint8_t f);
void generation(struct cellule c, struct labyrinth * labyrinth);
void put_ghost(labyrinth * labyrinth, int nb_ghosts);
void print_labyrinth(labyrinth * labyrinth);
void put_in_labyrinth(labyrinth * labyrinth, int nb, char obj);
void put_item(labyrinth * labyrinth, int nb_items);
void free_labyrinth(labyrinth * labyrinth);
int position_player(labyrinth * labyrinth, player * p);
int teleport_player(labyrinth * labyrinth, player * p, char * x, char * y);
int place_trap(labyrinth * labyrinth, char * x, char * y);
int nb_empty(labyrinth *l);
#endif // LABYRINTH

