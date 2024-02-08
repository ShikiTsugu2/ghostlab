#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/list_player.h"

list_player *create()
{
    list_player *l = malloc(sizeof(list_player));
    if (l == NULL)
    {
        exit(EXIT_FAILURE);
    }
    l->p = NULL;
    l->next = NULL;

    return l;
}

int list_isempty(list_player *l)
{
    if (l == NULL)
    {
        exit(EXIT_FAILURE);
    }
    return l->next == NULL;
}

int list_length(list_player *l)
{
    if (l == NULL)
    {
        exit(EXIT_FAILURE);
    }
    int i;
    for (i = 0, l = l->next; l != NULL; l = l->next, i++)
        ;
    return i;
}

int insertion(list_player *list, struct player * p)
{
    if (list == NULL)
    {
        return -1;
    }
    list_player *elt = malloc(sizeof(*elt));
    elt->p = p;
    elt->next = list->next;
    list->next = elt;
    return 0;
}

int suppression(list_player *l, struct player * p)
{
    list_player *precedant = l;
    list_player *courant = l->next;
    while (courant != NULL && strcmp(courant->p->id, p->id) && strcmp(courant->p->port, p->port))
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
player * get_player(list_player *l, char *id)
{
    list_player *courant = l->next;
    while (courant != NULL && strcmp(courant->p->id, id))
    {
        courant = courant->next;
    }
    if (courant == NULL)
    {
        return NULL;
    }

    return courant->p;
}
player * get_head_player(list_player *l)
{
    list_player *courant = l->next;
    if(courant==NULL){
        return NULL;
    }
    while (courant->next != NULL)
    {
        courant = courant->next;
    }

    return courant->p;
}
int delete_player(list_player *l, char *id, int f)
{
    list_player *precedant = l;
    list_player *courant = l->next;
    while (courant != NULL && strcmp(courant->p->id, id))
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
        free_list_players(courant);
    }
    return 0;
}


int exists_player(list_player *l, char *id)
{

    list_player *precedant = l;
    list_player *courant = l->next;
    while (courant != NULL && strcmp(courant->p->id, id))
    {
        precedant = courant;
        courant = courant->next;
    }
    if (courant == NULL)
    {
        return 0;
    }
    return 1;
}

void list_print(list_player *list){
    list_player *courant = list->next;
    while (courant != NULL)
    {
        printf("id: %s, port: %s\n", courant->p->id, courant->p->port);
        courant = courant->next;
    }
    return;
}

void free_list_players(list_player *l)
{
    while (l != NULL)
    {
        list_player *n = l->next;
        free_player(l->p);
        free(l);
        l = n;
    }
    
}

int ready(list_player *l)
{
    list_player *courant = l->next;
    while (courant != NULL)
    {
        if(courant->p->ready==0){
            return 0;
        }
        courant = courant->next;
    }

    return 1;
}

player * get_winner(list_player *l)
{
    list_player *winner= l->next;
    list_player *courant= l->next;
    while (courant != NULL)
    {
        if(atoi(winner->p->score) < atoi(courant->p->score)){
            winner = courant;
        }
        courant = courant->next;
    }

    return winner->p;
}



