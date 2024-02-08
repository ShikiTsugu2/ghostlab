#include "../../include/labyrinth.h"

labyrinth * create_labyrinth(uint16_t h, u_int16_t w, uint8_t f){
    struct labyrinth * labyrinth = malloc(sizeof(* labyrinth));
    if(labyrinth==NULL){
        return NULL;
    } 
    if(h<10){
        h = 10;
    }
    if(h>999){
        h = 999;
    }
    if(w<10){
        w = 10;
    }
    if(w>999){
        w = 999;
    }
    if(f==0){
        f = 1;
    }
    labyrinth->h = h;
    labyrinth->w = w;
    labyrinth->laby = malloc(sizeof(char *)*h);
    if(labyrinth->laby==NULL){
        return NULL;
    } 
    for(int i=0; i<h; i++){
        labyrinth->laby[i] =malloc(sizeof(char)*w);
        if(labyrinth->laby[i]==NULL){
            return NULL;
        } 
        if(i%2==0){
            for(int j=0; j<w; j++){
                if(j%2==1){
                    labyrinth->laby[i][j] = WALL;
                }
                else{
                    labyrinth->laby[i][j] = 'N';
                }
            }
        }
        else{
            for(int j=0; j<w; j++){
                labyrinth->laby[i][j] = WALL;
            }
        }
    }
    int x, y;
    srand(time(NULL));
    do{
        x = rand() % labyrinth->h;
        y = rand() % labyrinth->w;
    }
    while(labyrinth->laby[x][y]!='N');
    struct cellule cell;
    cell.x = x;
    cell.y = y;
    generation(cell, labyrinth);
    int n = nb_empty(labyrinth);
    if(n<=f){
        f = n - 5;
    }
    labyrinth->nb_ghosts = f;
    put_ghost(labyrinth, f);
    return labyrinth;
}

int nb_empty(labyrinth *l){
    int nb = 0;
    for(int i=0; i<l->h; i++){
        for(int j=0; j<l->w; j++){
            if(l->laby[i][j]==EMPTY){
                nb++;
            }
        }
    }
    return nb;
}
void generation(struct cellule c, struct labyrinth * labyrinth){
    labyrinth->laby[c.x][c.y] = EMPTY;
    struct cellule neighbours[4];
    int n = rand() % 4;
    for(int i=0; i<4; i++){
        if((n+i)%4==0 && c.x>1 && labyrinth->laby[c.x-2][c.y]=='N'){
            neighbours[0].x = c.x-2;
            neighbours[0].y = c.y;
            labyrinth->laby[c.x-1][c.y] = EMPTY;
            generation(neighbours[0], labyrinth);
        }
        if((n+i)%4==1 && c.x<labyrinth->h-2 && labyrinth->laby[c.x+2][c.y]=='N'){
            neighbours[1].x = c.x+2;
            neighbours[1].y = c.y;
            labyrinth->laby[c.x+1][c.y] = EMPTY;
            generation(neighbours[1], labyrinth);
        }
        if((n+i)%4==2 && c.y>1 && labyrinth->laby[c.x][c.y-2]=='N'){
            neighbours[2].x = c.x;
            neighbours[2].y = c.y-2;
            labyrinth->laby[c.x][c.y-1] = EMPTY;
            generation(neighbours[2], labyrinth);
        }
        if((n+i)%4==3 && c.y<labyrinth->w-2 && labyrinth->laby[c.x][c.y+2]=='N'){
            neighbours[3].x = c.x;
            neighbours[3].y = c.y+2;
            labyrinth->laby[c.x][c.y+1] = EMPTY;
            generation(neighbours[3], labyrinth);
        }
    }
}



void put_in_labyrinth(labyrinth * labyrinth, int nb, char obj){
    srand(time(NULL));
    int x;
    int y;
    for(int i=0; i<nb; i++){
        do{
            x = rand() % labyrinth->h;
            y = rand() % labyrinth->w;
        }
        while(labyrinth->laby[x][y]!=EMPTY);
        labyrinth->laby[x][y] = obj;
    }
}

void put_ghost(labyrinth * labyrinth, int nb_ghosts){
    put_in_labyrinth(labyrinth, nb_ghosts, GHOST);
}

void put_item(labyrinth * labyrinth, int nb_items){
    put_in_labyrinth(labyrinth, nb_items, ITEM);
}

void print_labyrinth(labyrinth * labyrinth){
    for(int i=0; i<labyrinth->h; i++){
        for(int j=0; j<labyrinth->w; j++){
            printf("%c", labyrinth->laby[i][j]);
        }
        printf("\n");
    }
}
int position_player(labyrinth * labyrinth, player * p){
    int x;
    int y;
    do{
        x = rand() % labyrinth->h;
        y = rand() % labyrinth->w;
    }
    while(labyrinth->laby[x][y]!=EMPTY);
    sprintf(p->x, "%03d", x);
    sprintf(p->y, "%03d", y);
}

int teleport_player(labyrinth * labyrinth, player * p, char * x, char * y){
    int ix = atoi(x);
    int iy = atoi(y);
    if(ix>=0 && ix<labyrinth->h && iy>=0 & iy<labyrinth->w && labyrinth->laby[ix][iy]==EMPTY){
        labyrinth->laby[atoi(p->x)][atoi(p->y)] = EMPTY;
        labyrinth->laby[ix][iy] = PLAYER;
        strcpy(p->x, x);
        strcpy(p->y, y);
        return 0;
    }
    return -1;
}

int place_trap(labyrinth * labyrinth, char * x, char * y){
    int ix = atoi(x);
    int iy = atoi(y);
    if(ix>=0 && ix<labyrinth->h && iy>=0 & iy<labyrinth->w && labyrinth->laby[ix][iy]==EMPTY){
        labyrinth->laby[ix][iy] = TRAP;
        return 0;
    }
    return -1;
}



void free_labyrinth(labyrinth * labyrinth){
    for(int i=0; i<labyrinth->h; i++){
        free(labyrinth->laby[i]);
    }
    free(labyrinth->laby);
    free(labyrinth);
}