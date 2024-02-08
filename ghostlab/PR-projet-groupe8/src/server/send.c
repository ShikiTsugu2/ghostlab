
#include "../../include/send.h"
int send_unrok(int socket, int m){
    char message[10];
    int position = 0;
    memcpy(message,"UNROK ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+sizeof(char)*6,&m, sizeof(uint8_t));
    position += sizeof(uint8_t);
    memcpy(message+sizeof(char)*6+sizeof(uint8_t),"***", sizeof(char)*3);
    position += sizeof(char)*3;
    if(send(socket, message, position, 0)==-1){
        perror("error send");
        return -1;
    }
    return 0;
}
int send_list_game(int socket, struct  list_game * games, pthread_mutex_t lock ){
    pthread_mutex_lock(&lock);
    uint8_t nb_games = list_game_length(games);
    pthread_mutex_unlock(&lock);
    char message[30*nb_games];
    int position = 0;
    memcpy(message+position,"GAMES ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+position, &nb_games, sizeof(uint8_t));
    position += sizeof(uint8_t);
    memcpy(message+position, "***", sizeof(char)*3);
    position += sizeof(char)*3;
    for(int i=0; i<nb_games; i++){
        games=games->next;
        memcpy(message+position,"OGAME ", sizeof(char)*6);
        position += sizeof(char)*6;
        pthread_mutex_lock(&lock);
        memcpy(message+position, &games->g->num, sizeof(uint8_t));
        pthread_mutex_unlock(&lock);
        position += sizeof(uint8_t);
        memcpy(message+position, " ", sizeof(char));
        position += sizeof(char);
        pthread_mutex_lock(&lock);
        memcpy(message+position, &games->g->nb_player, sizeof(uint8_t));
        pthread_mutex_unlock(&lock);
        position += sizeof(uint8_t);
        memcpy(message+position, "***", sizeof(char)*3);
        position += sizeof(char)*3;
        
    }

    message[position] = '\0';
    if(send(socket, message, position, 0)==-1){
        perror("error send");
        return -1;
    }
    return 0;
}

int send_list_player(int socket, struct game * game, uint8_t m, pthread_mutex_t lock){
    if(game==NULL){
        if(send(socket, "DUNNO***", 8, 0)==-1){
            perror("error send");
            return -1;
        }
        return 0;
    }
    char message[20*(game->nb_player+1)];
    int position = 0;
    memcpy(message+position,"LIST! ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+position, &m, sizeof(uint8_t));
    position += sizeof(uint8_t);
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    pthread_mutex_lock(&lock);
    memcpy(message+position, &game->nb_player, sizeof(uint8_t));
    pthread_mutex_unlock(&lock);
    position += sizeof(uint8_t);
    memcpy(message+position, "***", sizeof(char)*3);
    position += sizeof(char)*3;
    pthread_mutex_lock(&lock);
    list_player * players = game->players->next;
    for(int i=0; i<game->nb_player; i++){
        memcpy(message+position,"PLAYR ", sizeof(char)*6);
        position += sizeof(char)*6;
        memcpy(message+position, players->p->id, sizeof(char)*8);
        position += sizeof(char)*8;
        memcpy(message+position, "***", sizeof(char)*3);
        position += sizeof(char)*3;
        players = players->next;
    }
    pthread_mutex_unlock(&lock);
    message[position] = '\0';
    if(send(socket, message, position, 0)==-1){
        perror("error send");
        return -1;
    }
    return 0;
}


int register_to_aGame(int socket,struct list_game *games,char *message, pthread_mutex_t lock){
    /*Vérifier qu'il y'a un espace entre REGIS et id_player*/

    /*Récupérer id du joueur*/ 
    char  id_player[9];
    strncpy(id_player,message+6,8);
    id_player[8] = '\0';
    /*Vérifier qu'il n'y a aucun espace dans id_player*/

    
   /*Récupérer le port du joueur*/
    char port_player[5];
    strncpy(port_player,message+15,4);
    port_player[4] ='\0';
    /*Vérifier qu'il n'y a aucun espace dans port_player*/
   
    /*Vérifier qu'il y'a espace entre port joueur et id jeu*/
    uint8_t id_game=*(uint8_t*) &message[20];
    
    pthread_mutex_lock(&lock);
    game* game= get_game(games,id_game);
    if(game==NULL || exists_player(game->players, id_player)){
       if (send(socket,"REGNO***",8,0)==-1){
            perror("error send");
            return -1;
       }
       return 0;
    }
    pthread_mutex_unlock(&lock);
    struct player * p= create_player(id_player,port_player);
    pthread_mutex_lock(&lock);
    if (insertion (game->players,p)==-1){
        perror("error insertion player");
        return -1;
    }
    game->nb_player+=1; 
    pthread_mutex_unlock(&lock);
    
    char reply[20];
    int position=0; 
    memcpy(reply+position,"REGOK ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(reply+position, &id_game, sizeof(uint8_t));
    position += sizeof(uint8_t);
    memcpy(reply+position, "***", sizeof(char)*3);
    position += sizeof(char)*3;
    reply[position] = '\0';
    if (send(socket,reply,position,0)==-1){
        perror("error send");
        return -1;
    }
    return 1;
}



int send_size_labyrinth(int socket,struct list_game *games,char * message, pthread_mutex_t lock){

   	char id_game= message[6];
    uint8_t id = *(uint8_t*)&id_game;

    pthread_mutex_lock(&lock);
    game* game= get_game(games,id_game);
    if(game==NULL){
        if (send(socket,"DUNNO***",8,0)==-1){
            perror("error send");
            close(socket);
            return -1;
        }
        return 0;
    }
    pthread_mutex_unlock(&lock);   
    
    char reply[20];
    int position = 0;
    memcpy(reply+position,"SIZE! ",6);
    position+=sizeof(char)*6;
    memcpy(reply+position,&game->num,sizeof(uint8_t));
    position+=sizeof(uint8_t);
    memcpy(reply+position," ", sizeof(char));
    position += sizeof(char);

    pthread_mutex_lock(&lock);
    uint16_t h = htons(game->labyrinth->h);
    memcpy(reply+position,&h,sizeof(uint16_t));
    pthread_mutex_unlock(&lock);
    position+=sizeof(uint16_t);
    memcpy(reply+position," ", sizeof(char));
    position += sizeof(char);

    pthread_mutex_lock(&lock);
    uint16_t w = htons(game->labyrinth->w);
    memcpy(reply+position,&w,sizeof(uint16_t));
    pthread_mutex_unlock(&lock);

    position+=sizeof(uint16_t);
    memcpy(reply+position,"***",3);
    position+=sizeof(char)*3;
    reply[position]='\0';
    if (send(socket,reply,position,0)==-1){
            perror("error send");
            close(socket);
            return -1;
    }
    return 0;

}

int create_newGame(int socket,struct list_game *games,char *message,uint8_t compteur, char *ip, char *port, pthread_mutex_t lock){
    
    
    /*Récupérer id du joueur*/ 
    char  id_player[9];
    strncpy(id_player,message+6,8);
    id_player[8] = '\0'; 

   /*Récupérer le port du joueur*/
    char port_player[5];
    
    strncpy(port_player,message+15,4);
    port_player[4]='\0';
    if(is_numeric(port_player, 4)==0 || is_alpha_numeric(id_player, 8)==0){
        goto echec;
    }
    /*Créer un joueur*/
    struct player * p= create_player(id_player,port_player);
    /*Créer une partie*/
    struct game * g= create_game(compteur, p, ip, port); 
    /*Ajouter la partie à la liste des parties*/
    pthread_mutex_lock(&lock);
    if (list_game_insertion (games, g)==-1){  
        perror("insertion game");
    return -1;
    }
    pthread_mutex_unlock(&lock);
    
    /*Envoyer le message de réponse*/
    char reply[20];
    int position=0;
    memcpy(reply+position,"REGOK ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(reply+position, &(compteur), sizeof(uint8_t));
    position += sizeof(uint8_t);
    memcpy(reply+position, "***", sizeof(char)*3);
    position += sizeof(char)*3;
    reply[position] = '\0';
    if (send(socket,reply,position,0)==-1){
        perror("error send");
        return -1;
    }
    return 0;
    echec:
        if (send(socket,"REGNO***",8,0)==-1){
            perror("error send");
            return -1;
        }
        return 0;

}

int send_welco(int socket, game * game){
    char message[40];
    int position = 0;
    memcpy(message+position,"WELCO ", sizeof(char)*6);
    position += sizeof(char)*6;
    pthread_mutex_lock(&game->lock);
    memcpy(message+position,&game->num, sizeof(uint8_t));
    position += sizeof(uint8_t);
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    uint16_t h = htons(game->labyrinth->h);
    memcpy(message+position,&h, sizeof(uint16_t));
    position += sizeof(uint16_t);
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    uint16_t w = htons(game->labyrinth->w);
    memcpy(message+position,&w, sizeof(uint16_t));
    position += sizeof(uint16_t);
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position,&game->labyrinth->nb_ghosts, sizeof(uint8_t));
    position += sizeof(uint8_t);
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position,game->ip, sizeof(char)*15);
    position += sizeof(char)*15;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position,game->port, sizeof(char)*4);
    pthread_mutex_unlock(&game->lock);
    position += sizeof(char)*4;
    memcpy(message+position,"***", sizeof(char)*3);
    position += sizeof(char)*3;
    if (send(socket,message,position,0)==-1){
        perror("error send");
        return -1;
    }
    return 0;
}

int send_posit(int socket, player *p){
    char message[25];
    int position = 0;
    memcpy(message+position,"POSIT ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+position,p->id, sizeof(char)*8);
    position += sizeof(char)*8;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position,p->x, sizeof(char)*3);
    position += sizeof(char)*3;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position,p->y, sizeof(char)*3);
    position += sizeof(char)*3;
    memcpy(message+position,"***", sizeof(char)*3);
    position += sizeof(char)*3;
    if (send(socket,message,position,0)==-1){
        perror("error send");
        return -1;
    }
    return 0;
}
int send_score(char * ip, char *port, player *p){
    char message[40];
    int position = 0;
    memcpy(message+position,"SCORE ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+position, p->id, sizeof(char)*8);
    position += sizeof(char)*8;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position, p->score, sizeof(char)*4);
    position += sizeof(char)*4;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position, p->x, sizeof(char)*3);
    position += sizeof(char)*3;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position, p->y, sizeof(char)*3);
    position += sizeof(char)*3;
    memcpy(message+position,"+++", sizeof(char)*3);
    position += sizeof(char)*3;
    message[position] = '\0';
    return send_udp(ip, port, message);
}

int send_obtai(char * ip, char *port, player *p, int item){
    char message[22];
    int position = 0;
    memcpy(message+position,"OBTAI ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+position, p->id, sizeof(char)*8);
    position += sizeof(char)*8;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    if(item==0){
        memcpy(message+position, "BON", sizeof(char)*3);
    }else if(item==1){
        memcpy(message+position, "TEL", sizeof(char)*3);
    }else{
        memcpy(message+position, "TRA", sizeof(char)*3);
    }
    position += sizeof(char)*3;
    memcpy(message+position,"+++", sizeof(char)*3);
    position += sizeof(char)*3;
    message[position] = '\0';
    return send_udp(ip, port, message);
}
int send_penal(char * ip, char *port, player *p){
    char message[22];
    int position = 0;
    memcpy(message+position,"PENAL ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+position, p->id, sizeof(char)*8);
    position += sizeof(char)*8;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position,p->score, sizeof(char)*4);
    position += sizeof(char)*4;
    memcpy(message+position,"+++", sizeof(char)*3);
    position += sizeof(char)*3;
    message[position] = '\0';
    return send_udp(ip, port, message);
}

int send_udp(char * ip, char * port, char * message){
    int s = socket(PF_INET,SOCK_DGRAM,0);
    struct addrinfo *first_info;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype=SOCK_DGRAM;
    int n = 15;
    for(int i=0; i<15; i++){
        if(ip[i]=='#'){
            n--;
        }
    }
    char ip2[n+1];
    strncpy(ip2, ip, n);
    ip2[n] = '\0';
    if(getaddrinfo(ip2, port, &hints, &first_info)==0 && first_info!=NULL){
        struct sockaddr *saddr=first_info->ai_addr;
        if(sendto(s, message, strlen(message), 0, saddr, (socklen_t)sizeof(struct sockaddr_in))<=0){
            perror("error sendto");
            return -1;
        }
    }
    freeaddrinfo(first_info);
    return 0;
}



int send_endgame(game* game){
    list_player *plys = game->players;
    player* winner = get_winner(plys);
    char message[22];
    int position = 0;
    memcpy(message+position, "ENDGA ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+position, winner->id, sizeof(char)*8);
    position += sizeof(char)*8;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position, winner->score, sizeof(char)*4);
    position += sizeof(char)*4;
    memcpy(message+position,"+++", sizeof(char)*3);
    position += sizeof(char)*3;
    message[position] = '\0';
    if(send_udp(game->ip, game->port, message)==-1){
        perror("error send");
        return -1;
    }
}

int send_messa(char * ip, char * port, char *id, char * mess){
    char message[strlen(mess)+18];
    int position = 0;
    memcpy(message+position,"MESSA ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+position, id, sizeof(char)*8);
    position += sizeof(char)*8;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position, mess, strlen(mess));
    position += strlen(mess);
    memcpy(message+position,"+++", sizeof(char)*3);
    position += sizeof(char)*3;
    message[position] = '\0';
    if (send_udp(ip, port, message)==-1){
        perror("error send");
        return -1;
    }

}

int send_messp(char * ip, char * port, char *id, char * mess){
    char message[strlen(mess)+18];
    int position = 0;
    memcpy(message+position,"MESSP ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+position, id, sizeof(char)*8);
    position += sizeof(char)*8;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position, mess, strlen(mess));
    position += strlen(mess);
    memcpy(message+position,"+++", sizeof(char)*3);
    position += sizeof(char)*3;
    message[position] = '\0';
    if (send_udp(ip, port, message)==-1){
        perror("error send");
        return -1;
    }
}
int up_move(game * game, player * p, char *d){
    int distance = atoi(d);
    int x = atoi(p->x);
    int y = atoi(p->y);
    game->labyrinth->laby[x][y] = EMPTY;
    int score = atoi(p->score);
    int b = 0;
    int i;
    for(i=1; i<distance+1; i++){
        if(x-i<0 || game->labyrinth->laby[x-i][y]==WALL){
            break;
        }
        if(game->labyrinth->laby[x-i][y]==GHOST){
            b = 1;
            score += 50*p->bonus;
            if(p->bonus!=1){
                p->bonus *= 2;
            }
            sprintf(p->x, "%03d", x-i);
            sprintf(p->score, "%04d", score);
            game->labyrinth->laby[x-i][y] = EMPTY;
            game->labyrinth->nb_ghosts--;
            send_score(game->ip, game->port, p);
        }else if(game->labyrinth->laby[x-i][y]==ITEM){
            game->labyrinth->laby[x-i][y] = EMPTY;
            send_obtai(game->ip, game->port, p, give_item(p));
        }else if(game->labyrinth->laby[x-i][y]==TRAP && p->godmode==0){
            game->labyrinth->laby[x-i][y] = EMPTY;
            if(score<100){
                score = 0;
            }else{
                score -= 100;
            }
            i += 1;
            sprintf(p->score, "%04d", score);
            send_penal(game->ip, game->port, p);
            break;
        }
    }
    sprintf(p->x, "%03d", x-i+1);
    game->labyrinth->laby[x-i+1][y] = PLAYER;
    sprintf(p->score, "%04d", score);
    return b;
}

int down_move(game *game, player * p, char *d){
    int distance = atoi(d);
    int x = atoi(p->x);
    int y = atoi(p->y);
    game->labyrinth->laby[x][y] = EMPTY;
    int score = atoi(p->score);
    int b = 0;
    int i;
    for(i=1; i<distance+1; i++){
        if(x+i>game->labyrinth->h || game->labyrinth->laby[x+i][y]==WALL){
            break;
        }
        if(game->labyrinth->laby[x+i][y]==GHOST){
            b = 1;
            score += 50*p->bonus;
            if(p->bonus!=1){
                p->bonus *= 2;
            }
            sprintf(p->x, "%03d", x+i);
            sprintf(p->score, "%04d", score);
            game->labyrinth->laby[x+i][y] = EMPTY;
            game->labyrinth->nb_ghosts--;
            send_score(game->ip, game->port, p);
        }else if(game->labyrinth->laby[x+i][y]==ITEM){
            game->labyrinth->laby[x+i][y] = EMPTY;
            send_obtai(game->ip, game->port, p, give_item(p));
        }else if(game->labyrinth->laby[x+i][y]==TRAP && p->godmode==0){
            game->labyrinth->laby[x+i][y] = EMPTY;
            if(score<100){
                score = 0;
            }else{
                score -= 100;
            }
            i += 1;
            sprintf(p->score, "%04d", score);
            send_penal(game->ip, game->port, p);
            break;
        }
    }
    sprintf(p->x, "%03d", x+i-1);
    game->labyrinth->laby[x+i-1][y] = PLAYER;
    sprintf(p->score, "%04d", score);
    return b;
}

int left_move(game * game, player * p, char *d){
    int distance = atoi(d);
    int x = atoi(p->x);
    int y = atoi(p->y);
     game->labyrinth->laby[x][y] = EMPTY;
    int score = atoi(p->score);
    int b = 0;
    int i;
    for(i=1; i<distance+1; i++){
        if(y-i<0 || game->labyrinth->laby[x][y-i]==WALL){
            break;
        }
        if(game->labyrinth->laby[x][y-i]==GHOST){
            b = 1;
            score += 50*p->bonus;
            if(p->bonus!=1){
                p->bonus *= 2;
            }
            sprintf(p->y, "%03d", y-i);
            sprintf(p->score, "%04d", score);
            game->labyrinth->laby[x][y-i] = EMPTY;
            game->labyrinth->nb_ghosts--;
            send_score(game->ip, game->port, p);
        }else if(game->labyrinth->laby[x][y-i]==ITEM){
            game->labyrinth->laby[x][y-i] = EMPTY;
            send_obtai(game->ip, game->port, p, give_item(p));
        }else if(game->labyrinth->laby[x][y-i]==TRAP && p->godmode==0){
            game->labyrinth->laby[x][y-i] = EMPTY;
            if(score<100){
                score = 0;
            }else{
                score -= 100;
            }
            i += 1;
            sprintf(p->score, "%04d", score);
            send_penal(game->ip, game->port, p);
            break;
        }
    }
    sprintf(p->y, "%03d", y-i+1);
    game->labyrinth->laby[x][y-i+1]=PLAYER;
    sprintf(p->score, "%04d", score);
    return b;
}

int right_move(game *game, player * p, char *d){
    int distance = atoi(d);
    int x = atoi(p->x);
    int y = atoi(p->y);
    game->labyrinth->laby[x][y] = EMPTY;
    int score = atoi(p->score);
    int b = 0;
    int i;
    for(i=1; i<distance+1; i++){
        if(y+i>game->labyrinth->w || game->labyrinth->laby[x][y+i]==WALL){
            break;
        }
        if(game->labyrinth->laby[x][y+i]==GHOST){
            b = 1;
            score += 50*p->bonus;
            if(p->bonus!=1){
                p->bonus *= 2;
            }
            sprintf(p->y, "%03d", y+i);
            sprintf(p->score, "%04d", score);
            game->labyrinth->laby[x][y+i] = EMPTY;
            game->labyrinth->nb_ghosts--;
            send_score(game->ip, game->port, p);
        }else if(game->labyrinth->laby[x][y+i]==ITEM){
            game->labyrinth->laby[x][y+i] = EMPTY;
            send_obtai(game->ip, game->port, p, give_item(p));
        }else if(game->labyrinth->laby[x][y+i]==TRAP && p->godmode==0){
            game->labyrinth->laby[x][y+i] = EMPTY;
            if(score<100){
                score = 0;
            }else{
                score -= 100;
            }
            i += 1;
            sprintf(p->score, "%04d", score);
            send_penal(game->ip, game->port, p);
            break;
        }
    }
    sprintf(p->y, "%03d", y+i-1);
    game->labyrinth->laby[x][y+i-1] = PLAYER;
    sprintf(p->score, "%04d", score);
    return b;
}
int send_move(game* game, player* p, int socket, char* message){
    char dist[4];
    memcpy(dist,&message[6],3);
    dist[3] = '\0';
    int ghost = 0;
    pthread_mutex_lock(&game->lock);
    if(strncmp(message, "UPMOV", 5)==0){
        ghost = up_move(game,p,dist);
    }else if(strncmp(message, "DOMOV", 5)==0){
        ghost = down_move(game,p,dist);
    }else if(strncmp(message, "RIMOV", 5)==0){
        ghost = right_move(game,p,dist);
    }else if(strncmp(message, "LEMOV", 5)==0){
        ghost = left_move(game,p,dist);
    }
    pthread_mutex_unlock(&game->lock);
    p->bonus = 1;
    if(ghost == 0){
        char buff[16];
        int ind = 0;
        memcpy(buff+ind, "MOVE! ", sizeof(char)*6);
        ind += sizeof(char)*6;
        memcpy(buff+ind, p->x, sizeof(char)*3);
        ind += sizeof(char)*3;
        memcpy(buff+ind, " ", sizeof(char));
        ind += sizeof(char);
        memcpy(buff+ind, p->y, sizeof(char)*3);
        ind += sizeof(char)*3;
        memcpy(buff+ind, "***", sizeof(char)*3);
        ind += sizeof(char)*3;
        if (send(socket,buff,ind,0)==-1){
            perror("error send");
            return -1;
        }
    }else{
        char buff[21];
        int ind = 0;
        memcpy(buff+ind, "MOVEF ", sizeof(char)*6);
        ind += sizeof(char)*6;
        memcpy(buff+ind, p->x, sizeof(char)*3);
        ind += sizeof(char)*3;
        memcpy(buff+ind, " ", sizeof(char));
        ind += sizeof(char);
        memcpy(buff+ind, p->y, sizeof(char)*3);
        ind += sizeof(char)*3;
        memcpy(buff+ind, " ", sizeof(char));
        ind += sizeof(char);
        memcpy(buff+ind, p->score, sizeof(char)*4);
        ind += sizeof(char)*4;
        memcpy(buff+ind, "***", sizeof(char)*3);
        ind += sizeof(char)*3;
        if (send(socket,buff,ind,0)==-1){
                perror("error send");
                return -1;
        }
        pthread_mutex_lock(&game->lock);
        if(game->labyrinth->nb_ghosts == 0){
            send_endgame(game);
        }
        pthread_mutex_unlock(&game->lock);
    }
    printf("\n");
    print_labyrinth(game->labyrinth);
    return 0;
}

int send_item(player * p, int socket, pthread_mutex_t lock){
    char message[15];
    int position = 0;
    memcpy(message+position,"ITEM! ", sizeof(char)*6);
    position += sizeof(char)*6;
    pthread_mutex_lock(&lock);
    memcpy(message+position, &p->items[0], sizeof(uint8_t));
    pthread_mutex_unlock(&lock);
    position += sizeof(uint8_t);
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    pthread_mutex_lock(&lock);
    memcpy(message+position, &p->items[1], sizeof(uint8_t));
    pthread_mutex_unlock(&lock);
    position += sizeof(uint8_t);
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    pthread_mutex_lock(&lock);
    memcpy(message+position,&p->items[2], sizeof(uint8_t));
    pthread_mutex_unlock(&lock);
    position += sizeof(uint8_t);
    memcpy(message+position,"***", sizeof(char)*3);
    position += sizeof(char)*3;
    if (send(socket,message,position,0)==-1){
        perror("error send");
        return -1;
    }
    return 0;
}

int send_itbon(int socket, player * p){
    if(p->bonus!=1){
        if(send(socket,"DUNNO***",8,0)==-1){
                perror("error send");
                return -1;
        }
        return 0;
    }
    if(p->items[0]==0){
        if(send(socket,"NOITM***",8,0)==-1){
            perror("error send");
            return -1;
        }
        return 0;
    }
    p->bonus = 2;
    if(p->itemInf == 0){
        p->items[0] -= 1;
    }
    if(send(socket,"BONOK***",8,0)==-1){
        perror("error send");
        return -1;
    }
    return 0;
}

int send_ittel(int socket, player *p, game * g, char *message){
    if(p->items[1]==0){
        if(send(socket,"NOITM***",8,0)==-1){
            perror("error send");
            return -1;
        }
    }else{
        char x[4];
        char y[4];
        strncpy(x, message+6, 3);
        x[3] = '\0';
        strncpy(y, message+10, 3);
        y[3] = '\0';
        pthread_mutex_lock(&g->lock);
        int tel = teleport_player(g->labyrinth, p, x, y);
        pthread_mutex_unlock(&g->lock);
        if(tel==-1){
            if(send(socket,"DUNNO***",8,0)==-1){
                perror("error send");
                return -1;
            }
        }else{
            if(p->itemInf == 0){
                p->items[1] -= 1;
            }
            int position = 0;
            char mess[17];
            memcpy(mess+position,"TELOK ", sizeof(char)*6);
            position += sizeof(char)*6;
            pthread_mutex_lock(&g->lock);
            memcpy(mess+position,p->x, sizeof(char)*3);
            position += sizeof(char)*3;
            memcpy(mess+position," ", sizeof(char));
            position += sizeof(char);
            memcpy(mess+position,p->y, sizeof(char)*3);
            pthread_mutex_unlock(&g->lock);
            position += sizeof(char)*3;
            memcpy(mess+position,"***", sizeof(char)*3);
            position += sizeof(char)*3;
            if (send(socket,mess,position,0)==-1){
                perror("error send");
                return -1;
            }
        }
    }
    return 0;
}

int send_ittra(int socket, player *p, game * g, char *message){
    if(p->items[2]==0){
        if(send(socket,"NOITM***",8,0)==-1){
            perror("error send");
            return -1;
        }
    }else{
        char x[4];
        char y[4];
        strncpy(x, message+6, 3);
        x[3] = '\0';
        strncpy(y, message+10, 3);
        y[3] = '\0';
        pthread_mutex_lock(&g->lock);
        int tra = place_trap(g->labyrinth, x, y);
        pthread_mutex_unlock(&g->lock);
        if(tra==-1){
            if(send(socket,"DUNNO***",8,0)==-1){
                perror("error send");
                return -1;
            }
        }else{
            if(p->itemInf == 0){
                p->items[2] -= 1;
            }
            int position = 0;
            char mess[17];
            memcpy(mess+position,"TRAOK ", sizeof(char)*6);
            position += sizeof(char)*6;
            memcpy(mess+position,x, sizeof(char)*3);
            position += sizeof(char)*3;
            memcpy(mess+position," ", sizeof(char));
            position += sizeof(char);
            memcpy(mess+position,y, sizeof(char)*3);
            position += sizeof(char)*3;
            memcpy(mess+position,"***", sizeof(char)*3);
            position += sizeof(char)*3;
            if(send(socket,mess,position,0)==-1){
                perror("error send");
                return -1;
            }
        }
    }
    return 0;
}

int send_FGONE(char * ip, char *port, player *p){
    char message[22];
    int position = 0;
    memcpy(message+position,"FGONE ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+position, p->id, sizeof(char)*8);
    position += sizeof(char)*8;
    memcpy(message+position," ", sizeof(char));
    position += sizeof(char);
    memcpy(message+position, p->score, sizeof(char)*4);
    position += sizeof(char)*4;
    memcpy(message+position,"+++", sizeof(char)*3);
    position += sizeof(char)*3;
    message[position] = '\0';
    return send_udp(ip, port, message);
}

int send_killg(game* game, player* player, int socket){
    labyrinth* lab = game->labyrinth;
    int nbFantIni = lab->nb_ghosts;
    int score = atoi(player->score);
    pthread_mutex_lock(&game->lock);
    for(int i = 0; i<lab->h; i++){
        for(int j = 0; j<lab->w; j++){
            if(lab->laby[i][j] == GHOST){
                lab->laby[i][j] = EMPTY;
            }
        }
    }
    lab->nb_ghosts = 0;
    score = nbFantIni*50;
    sprintf(player->score, "%04d", score);
    pthread_mutex_unlock(&game->lock);
    char buff[22];
    int ind = 0;
    memcpy(buff+ind, "KILAL ", sizeof(char)*6);
    ind += sizeof(char)*6;
    pthread_mutex_lock(&game->lock);
    memcpy(buff+ind, player->id, sizeof(char)*8);
    ind += sizeof(char)*8;
    memcpy(buff+ind, " ", sizeof(char));
    ind += sizeof(char);
    memcpy(buff+ind, player->score, sizeof(char)*4);
    pthread_mutex_unlock(&game->lock);
    ind += sizeof(char)*4;
    memcpy(buff+ind, "***", sizeof(char)*3);
    ind += sizeof(char)*3;
    if (send(socket,buff,ind,0)==-1){
        perror("error send");
        return -1;
    }
    pthread_mutex_lock(&game->lock);
    send_FGONE(game->ip,game->port,player);
    if(lab->nb_ghosts == 0){
        send_endgame(game);
    }
    pthread_mutex_unlock(&game->lock);
}

int send_labsh(game* game, int socket){
    pthread_mutex_lock(&game->lock);
    labyrinth *lab = game->labyrinth;
    char message[15+sizeof(char)*lab->h*lab->w];
     pthread_mutex_unlock(&game->lock);
    int position = 0;
    memcpy(message+position,"SHOW! ", sizeof(char)*6);
    position += sizeof(char)*6;
    pthread_mutex_lock(&game->lock);
    uint16_t h = htons(lab->h);
    pthread_mutex_unlock(&game->lock);
    memcpy(message+position, &h, sizeof(uint16_t));
    position += sizeof(uint16_t);
    memcpy(message+position, " ", sizeof(char));
    position += sizeof(char);
    pthread_mutex_lock(&game->lock);
    uint16_t w = htons(lab->w);
    pthread_mutex_unlock(&game->lock);
    memcpy(message+position, &w, sizeof(uint16_t));
    position += sizeof(uint16_t);
    memcpy(message+position, " ", sizeof(char));
    position += sizeof(char);
    pthread_mutex_lock(&game->lock);
    for(int i = 0; i<lab->h; i++){
        memcpy(message+position, lab->laby[i], sizeof(char)*lab->w);
        position += sizeof(char)*lab->w;
    }
    pthread_mutex_unlock(&game->lock);
    memcpy(message+position,"***", sizeof(char)*3);
    position += sizeof(char)*3;
    if (send(socket,message,position,0)==-1){
        perror("error send");
        return -1;
    }
    return 0;
}

int send_givch(int socket, player* sender, player* reciever){
    char message[17];
    int position = 0;
    memcpy(message+position,"GAVE! ", sizeof(char)*6);
    position += sizeof(char)*6;
    memcpy(message+position, reciever->id, sizeof(char)*8);
    position += sizeof(char)*8;
    memcpy(message+position,"***", sizeof(char)*3);
    position += sizeof(char)*3;
    if(send(socket,message,position,0)==-1){
        perror("error send");
        return -1;
    }
    if(send_messp(reciever->ip, reciever->port, sender->id, "Vous avez le mode CHEAT désormais actif.")==-1){
        perror("error send");
        return -1;
    }
    reciever->cheat=1;
    return 0;
}

int send_list_player_in_game(game* game,int socket){
    char message[50*(game->nb_player+1)];
    int position = 0;
    memcpy(message+position,"GLIS! ", sizeof(char)*6);
    position += sizeof(char)*6;
    pthread_mutex_lock(&game->lock);
    memcpy(message+position, &game->nb_player, sizeof(uint8_t));
    pthread_mutex_unlock(&game->lock);
    position += sizeof(uint8_t);
    memcpy(message+position, "***", sizeof(char)*3);
    position += sizeof(char)*3;
    
    list_player * players = game->players->next;
    pthread_mutex_lock(&game->lock);
    for(int i=0; i<game->nb_player; i++){
        memcpy(message+position,"GPLYR ", sizeof(char)*6);
        position += sizeof(char)*6;
        memcpy(message+position, players->p->id, sizeof(char)*8);
        position += sizeof(char)*8;
        memcpy(message+position, " ", sizeof(char));
        position += sizeof(char);
        memcpy(message+position, players->p->x, sizeof(char)*3);
        position += sizeof(char)*3;
        memcpy(message+position, " ", sizeof(char));
        position += sizeof(char);
        memcpy(message+position, players->p->y, sizeof(char)*3);
        position += sizeof(char)*3;
        memcpy(message+position, " ", sizeof(char));
        position += sizeof(char);
        memcpy(message+position, players->p->score, sizeof(char)*4);
        position += sizeof(char)*4;
        memcpy(message+position, "***", sizeof(char)*3);
        position += sizeof(char)*3;
        players = players->next;
    }
    pthread_mutex_unlock(&game->lock);
    message[position] = '\0';
    if(send(socket, message, position, 0)==-1){
        perror("error sendplayers");
        return -1;
    }
    return 0;
}

int send_gobye(int socket, player * p, game* game){
    if(send(socket,"GOBYE***",8,0)==-1){
        perror("error send");
        return -1;
    }
    pthread_mutex_lock(&game->lock);
    list_player * players = game->players;
    if (delete_player(players, p->id, 1)==-1){
        perror("error delete");
        return -1;
    }
    game->nb_player -= 1;
    pthread_mutex_unlock(&game->lock);
    close(socket);
    return 0;
}

int send_resize_labyrinth(int socket, game *game,char * message){

    uint16_t h ;
    uint16_t w ;
    uint8_t nb_ghosts;
    int position = 6;
    memcpy(&h, message+position, sizeof(uint16_t));
    position += sizeof(uint16_t)+1;
    h=ntohs(h);
    memcpy(&w, message+position, sizeof(uint16_t));
    w=ntohs(w);
    position += sizeof(uint16_t)+1;
    memcpy(&nb_ghosts, message+position, sizeof(uint8_t));
    position += sizeof(uint8_t);
    pthread_mutex_lock(&game->lock);
    labyrinth * lab = create_labyrinth(h,w,nb_ghosts);
    if(lab==NULL){
        perror("error create labyrinth");
        return -1;
    }
    free_labyrinth(game->labyrinth);
    game->labyrinth=lab;
    pthread_mutex_unlock(&game->lock);
    if(send(socket,"RESOK***",8,0)==-1){
        perror("error send Reply");
        return -1;
    }

    return 0;
}