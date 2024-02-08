#ifndef PLAYER_H
#define PLAYER_H

#include <arpa/inet.h>
#include <time.h>

typedef struct player player;
struct player
{
    char id[9];
    char port[5];
    char x[4];
    char y[4];
    char score[5];
    char ip[16];
    int items[3]; //item[0] : Bonus | items[1] : Téléporteur | items[2] : Piège
    int cheat; //cheat = 0, cheat mode off sinon si cheat = 1, cheat mode on
    int godmode; //godmode = 0, god mode off sinon si godmode = 1, god mode mode on
    int itemInf; //itemInf = 0, item infini mode off sinon si itemInf = 1, item infini mode on
    int ready;
    int bonus;
};

player * create_player(char id[9],char port [5]);
int give_item(player * p);
void free_player(player * p);
#endif // PLAYER
