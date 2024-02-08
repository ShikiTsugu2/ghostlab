#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/game.h"

struct game * create_game(uint8_t num, player * p, char *ip, char * port ){

    struct game *g = malloc(sizeof(* g));
    if(g==NULL){
        return NULL;
    }
    g->items = 0;
    g->num = num;
    g->nb_player = 1;
    strcpy(g->ip, "###############");
    strncpy(g->ip, ip, strlen(ip));
    g->ip[15] ='\0';
    strncpy(g->port, port, 4);
    g->port[4] = '\0';
    g->players = create();
    pthread_mutex_init(&g->lock, NULL); 
    pipe(g->pipes);
    if(insertion(g->players,p) == -1){
        exit(EXIT_FAILURE);
    }
    //Pour le moment j'ai mis hauteur et largeur à 10 après on pourra changer
    g->labyrinth = create_labyrinth(DEFAULT_H, DEFAULT_W, DEFAULT_F);
    return g;

}

void free_game(game * g){
    if(g!=NULL){
        free_labyrinth(g->labyrinth);
        free_list_players(g->players);
        free(g);
    }
}
