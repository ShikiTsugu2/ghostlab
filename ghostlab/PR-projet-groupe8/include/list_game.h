#ifndef LIST_GAME_H 
#define LIST_GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

typedef struct list_game list_game;

struct list_game
{
    struct game *g;
    list_game *next;

};


//Main functions:
/**Crée une liste vide avec le premiere élément ne contient pas de valeur (joueur) .**/
list_game *create_list_game();
/**renvoie la longueur de la liste**/
uint8_t list_game_length(list_game *l);

/**renvoie 1 si la liste est vide sinon 0 **/
int list_game_isempty(list_game *l);

/**Insère la partie g si l'insertion se passe bien renvoie 0
 * sinon -1**/
int list_game_insertion(list_game *list,struct game *g);


/**Supprime la partie g si la suppression se passe bien renvoie 0
 * sinon -1**/
int list_game_suppression (list_game *list,struct game *g);

/**La fonction renvoie 1 si la partie g est déjà dans la liste list
 * sinon 0**/
int list_game_exists(list_game *list,struct game *g);

/**Affiche le contenu de la liste list**/
void list_game_print(list_game *list);

/**Libère la mémoire alloué par l**/
void list_game_destroy(list_game *l);

game * get_game(list_game * games, uint8_t m);

int delete_game(list_game *l, uint8_t m, int f);
void free_list_games(list_game * l);



#endif //LIST_GAME
