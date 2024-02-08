#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/player.h"


struct player * create_player(char id[9],char port [5]){
     struct player *p = malloc(sizeof(* p));
     
     strncpy(p->id,id,8);
     p->id[8]='\0';
     
     strncpy(p->port,port,4);
     p->port[4]='\0';
    
    strncpy(p->score, "0000", 4);
    p->score[4] = '\0';

     p->ready = 0;
     p->items[0] = 0;
     p->items[1] = 0;
     p->items[2] = 0;
     p->cheat = 0;
     p->godmode = 0;
     p->itemInf = 0;
     p->bonus = 1;
     return p;

}

int give_item(player * p){
    srand(time(NULL));
    int n = rand() % 3;
    p->items[n] += 1;
    return n;
}

void free_player(player * p){
    if(p!=NULL){
        free(p);
    }
}