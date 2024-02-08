#ifndef LIST_PLAYER_H 
#define LIST_PLAYER_H


#include "player.h"

typedef struct list_player list_player;

struct list_player
{
    struct player * p;
    list_player *next;
};


//Main functions:
/**Crée une liste vide avec le premiere élément ne contient pas de valeur (joueur) .**/
list_player *create();
/**renvoie la longueur de la liste**/
int list_length(list_player *l);

/**renvoie 1 si la liste est vide sinon 0 **/
int list_isempty(list_player *l);

/**Insère le joueur p si l'insertion se passe bien renvoie 0
 * sinon -1**/
int insertion(list_player *list,struct player * p);


/**Supprime le joueur p si la suppression se passe bien renvoie 0
 * sinon -1**/
int suppression (list_player *list,struct player * p);

/**La fonction renvoie 1 si le joueur p est déjà dans la list list
 * sinon 0**/
int exists(list_player *list,struct player * p);

/**Affiche le contenu de la liste list**/
void list_print(list_player *list);

/**Libère la mémoire alloué par l**/
void free_list_players(list_player *l);

int delete_player(list_player *l, char *id, int f);

player * get_player(list_player *l, char *id);

int ready(list_player *l);
int exists_player(list_player *l, char *id);
player * get_head_player(list_player *l);
player * get_winner(list_player *l);

#endif //LIST_PLAYER
