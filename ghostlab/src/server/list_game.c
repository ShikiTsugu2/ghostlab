#include "../../include/list_game.h"

list_game *create_list_game()
{
    list_game *l = malloc(sizeof(list_game));
    if (l == NULL)
    {
        exit(EXIT_FAILURE);
    }
    l->g = NULL;
    l->next = NULL;
    return l;
}

int list_game_isempty(list_game *l)
{
    if (l == NULL)
    {
        exit(EXIT_FAILURE);
    }
    return l->next == NULL;
}

uint8_t list_game_length(list_game *l)
{
    if (l == NULL)
    {
        exit(EXIT_FAILURE);
    }
    uint8_t i;
    for (i = 0, l = l->next; l != NULL; l = l->next, i++)
        ;
    return i;
}

int list_game_insertion(list_game *list, struct game *g)
{
    if (list == NULL)
    {
        return -1;
    }

    list_game *elt = malloc(sizeof(* elt));
    elt->g = g;

    elt->next = list->next;
    list->next = elt;
    return 0;
}

int list_game_suppression(list_game *l, struct game *g)
{
    list_game *precedant = l;
    list_game *courant = l->next;
    while (courant != NULL && courant->g->num != g->num)
    {
        precedant = courant;
        courant = courant->next;
    }
    if (courant == NULL)
    {
        return -1;
    }

    precedant->next = courant->next;
    free(courant);
    return 0;
}

int delete_game(list_game *l, uint8_t m, int f){
    list_game *precedant = l;
    list_game *courant = l->next;
    while (courant != NULL && courant->g->num != m)
    {
        precedant = courant;
        courant = courant->next;
    }
    if (courant == NULL)
    {
        return -1;
    }

    precedant->next = courant->next;
    courant->next = NULL;
    if(f){
        free_list_games(courant);
    }else{
        free(courant);
    }
    
    return 0;

}

int list_game_exists(struct list_game *list, struct game *g)
{
	
    list_game *courant = list->next;
    while (courant != NULL)
    {
        if (courant->g->num == g->num)
        {
            return 1;
        }
        courant = courant->next;
    }
    return 0;
}

void list_game_print(struct list_game *list){
    list_game *courant = list->next;
    while (courant != NULL)
    {
        printf("Numéro de la partie : %u\nNombre de joueurs : %u\n", courant->g->num, courant->g->nb_player);
        courant = courant->next;
    }
    return;
}

void list_game_destroy(list_game *l)
{
    while (l != NULL)
    {
        list_game *n = l->next;
        free(l->g);
        free(l);
        l = n;
    }
    return;
}

game * get_game(list_game * games, uint8_t m){
    if (games == NULL)
    {
        exit(EXIT_FAILURE);
    }
    uint8_t n = list_game_length(games);
    for(int i=0; i<n; i++){
        games = games->next;
        if(games->g->num==m){
            return  games->g;
        }
    }
    return NULL;

}

void free_list_games(list_game * l){
    while (l != NULL)
    {
        list_game *n = l->next;
        free_game(l->g);
        free(l);
        l = n;
    }
}