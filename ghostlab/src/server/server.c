#include "../../include/server.h"

uint8_t nb_games = 1;
struct list_game * games_no_start;
struct list_game * games_start;
char port[5];
struct in_addr ip_multicast;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * move_ghosts(void *g){
    game * game = (struct game *) g;
    char message[20];
    int b = 0;
    while(game->labyrinth->nb_ghosts!=0){
        struct pollfd poll_fds[1];
        poll(poll_fds, 0, 60000);
        pthread_mutex_lock(&game->lock);
        if(game->labyrinth->nb_ghosts!=0){
            for(int i=0; i<game->labyrinth->h; i++){
                for(int j=0; j<game->labyrinth->w; j++){
                    if(game->labyrinth->laby[i][j]==GHOST){
                        game->labyrinth->laby[i][j] = EMPTY;
                        b = 1;
                        break;
                    }
                }
                if(b){
                    b = 0;
                    break;
                }
            }
            int x, y;
            do{
                x = rand() % game->labyrinth->h;
                y = rand() % game->labyrinth->w;
            }
            while(game->labyrinth->laby[x][y]!=EMPTY);
            game->labyrinth->laby[x][y] = GHOST;
            sprintf(message, "GHOST %03d %03d+++", x, y);
            send_udp(game->ip, game->port, message);
            
        }
        pthread_mutex_unlock(&game->lock);
    }
    return NULL;
}

//valgrind --leak-check=full --log-file="valgrind.log" ./server 2718
//valgrind --tool='helgrind'
void * connexion(void *info){
    char message[1024];
    message[0] ='a';
    message[1]='\0';
    uint8_t registered = 0;
    char  id_player[9];
    id_player[0] = 'a';
    id_player[1] = '\0';
    int socket_client;
    memcpy(&socket_client, info, sizeof(int));
    struct in_addr addr;
    memcpy(&addr, info+sizeof(int), sizeof(addr));
    free(info);
    
    if(send_list_game(socket_client, games_no_start, lock)==-1){
        return NULL;
    }
    int recu;
    while(strcmp(message, "START***") || registered==0){
        recu = 0;
        memset(message, 0, sizeof(char)*1024);
        while(recu<50 && strncmp(message+recu-3, "***", 3)!=0){
            recu += recv(socket_client, message+recu, 50-recu, 0);
        }
        if(recu<=0){
            if(registered!=0){
                pthread_mutex_lock(&lock);
                game * g = get_game(games_no_start, registered);
                delete_player(g->players, id_player, 1);
                g->nb_player -= 1;
                pthread_mutex_unlock(&lock);
            }
            perror("error recv");
            return NULL;
        }
        message[recu] = '\0';
        if(VERB){
            printf("[ %s ]\n", message);
        }
        if(strncmp(message, "NEWPL", 5)==0){
            if(check_newpl(message, recu)==0){
                goto error;
            }
            if(is_numeric(message+15, 4)==0 || is_alpha_numeric(message+6, 8)==0 || registered!=0){
                if (send(socket_client,"REGNO***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
                continue;
            }
            else if(create_newGame(socket_client,games_no_start,message,nb_games, inet_ntoa(ip_multicast), port, lock)==-1){
                perror("NEWPL");
                return NULL;
            }
            ip_multicast.s_addr = htonl(ntohl(ip_multicast.s_addr) + 1);
            registered = nb_games;
            strncpy(id_player,message+6,8);
            id_player[8] = '\0';
            nb_games+=1;
        }  
        else if(strncmp(message, "REGIS", 5)==0){
            if(check_regis(message, recu)==0){
                goto error;
            }
            if(is_numeric(message+15, 4)==0 || is_alpha_numeric(message+6, 8)==0 || registered!=0){
                if (send(socket_client,"REGNO***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
                continue;
            }
            int exist = register_to_aGame(socket_client,games_no_start,message, lock);
            if(exist==-1){
                perror("REGIS");
                return NULL;
            }
            if(exist==1){
                registered=*(uint8_t*) &message[20];
                strncpy(id_player,message+6,8);
                id_player[8] = '\0';
            }
        }
        else if(strncmp(message, "RESIZ",5)==0){
            if(check_resiz(message, recu)==0){
                goto error;
            }
            if(registered==0){
                if (send(socket_client,"DUNNO***",8,0)==-1){
                    perror("error send resiz");
                    return NULL;
                }
            }
            else{
                pthread_mutex_lock(&lock);
                game * g = get_game(games_no_start, registered);
                player * head = get_head_player(g->players);
                pthread_mutex_unlock(&lock);
                if(strncmp(id_player, head->id, 8)!=0 ){
                    if(send(socket_client,"DUNNO***",8,0)==-1){
                        perror("error send resiz");
                        return NULL;
                    }
                    continue;
                }else{
                    
                     if(send_resize_labyrinth(socket_client,g,message)==-1){
                        perror("error send");
                        return NULL;
                    }
                }
                
        }
        }
        else if(strncmp(message, "GAME?", 5)==0){
            if(check_game(message, recu)==0){
                goto error;
            }
            send_list_game(socket_client, games_no_start, lock);
        }
        else if(strncmp(message, "LIST?", 5)==0){
            if(check_list(message, recu)==0){
                goto error;
            }
            uint8_t m = 0;
            memcpy(&m,message+6, sizeof(char));
            pthread_mutex_lock(&lock);
            game * g = get_game(games_no_start, m);
            pthread_mutex_unlock(&lock);
            send_list_player(socket_client, g, m, lock);
        }
        else if(strncmp(message, "SIZE?", 5)==0){
            if(check_size(message, recu)==0){
                goto error;
            }
            if (send_size_labyrinth(socket_client,games_no_start,message, lock)==-1){
            	perror("SIZE?");
            	return NULL;
            }
            
        }
        else if(strcmp(message, "UNREG***")==0){
            if(check_unreg(message, recu)==0){
                goto error;
            }
            if(registered==0){
                if (send(socket_client,"DUNNO***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                pthread_mutex_lock(&lock);
                game * g = get_game(games_no_start, registered);
                if(delete_player(g->players, id_player, 1)==-1){
                    if(send(socket_client,"DUNNO***",8,0)==-1){
                        perror("error send");
                        return NULL;
                    }
                    continue;
                }
                g->nb_player -= 1;
                pthread_mutex_unlock(&lock);
                
                if(send_unrok(socket_client, registered)==-1){
                    perror("UNROK");
                    return NULL;
                }
                registered = 0;
            }
        } 
        else if(strcmp(message, "ITMON***")==0){
            if(check_itmon(message, recu)==0){
                goto error;
            }
            if(registered==0){
                if (send(socket_client,"DUNNO***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
            else{
                pthread_mutex_lock(&lock);
                game * g = get_game(games_no_start, registered);
                player * head = get_head_player(g->players);
                pthread_mutex_unlock(&lock);
                if(strncmp(id_player, head->id, 8)!=0 ){
                    if(send(socket_client,"DUNNO***",8,0)==-1){
                        perror("error send");
                        return NULL;
                    }
                    continue;
                }else{
                    g->items = 1;
                     if(send(socket_client,"ITMOK***",8,0)==-1){
                        perror("error send");
                        return NULL;
                    }
                }
                
            }
        }
        else if(strcmp(message, "CHEAT***")==0){
            if(check_cheat(message, recu)==0){
                goto error;
            }
            if(registered==0){
                if (send(socket_client,"DUNNO***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
            else{
                pthread_mutex_lock(&lock);
                game * g = get_game(games_no_start, registered);
                player * head = get_head_player(g->players);
                pthread_mutex_unlock(&lock);
                if(strncmp(id_player, head->id, 8)!=0 ){
                    if(send(socket_client,"DUNNO***",8,0)==-1){
                        perror("error send");
                        return NULL;
                    }
                    continue;
                }else{
                    head->cheat = 1;
                    if(send(socket_client,"CHTOK***",8,0)==-1){
                        perror("error send");
                        return NULL;
                    }
                }
                
            }
        }
        else if(strcmp(message, "START***")==0 ){
            if(check_start(message, recu)==0){
                goto error;
            }
            if(registered==0){
                printf("Message START reçu avant d'être inscrit à une partie: %s\n", message);
                close(socket_client);
                return NULL;
            }
        }
        else{
            printf("Message reçu non conforme : %s\n", message);
            close(socket_client);
            return NULL;
        }
    } 
    pthread_mutex_lock(&lock);
    game * game_play = get_game(games_no_start, registered);
    player * player = get_player(game_play->players, id_player);
    player->ready = 1;
    strcpy(player->ip, "###############");
    strncpy(player->ip, inet_ntoa(addr), strlen(inet_ntoa(addr)));
    
    if(ready(game_play->players)){
        delete_game(games_no_start, registered, 0);
        list_game_insertion(games_start, game_play);
        pthread_t th;
        pthread_create(&th,NULL, move_ghosts, game_play);
        if(game_play->items){
            put_item(game_play->labyrinth, (game_play->labyrinth->h*game_play->labyrinth->w)/20);
        }
        write(game_play->pipes[1],"a",1);
        pthread_mutex_unlock(&lock);
        
        
    }else{
        pthread_mutex_unlock(&lock);
        struct pollfd poll_fds[1];
        poll_fds[0].fd = game_play->pipes[0];
        poll_fds[0].events = POLLIN;
        poll(poll_fds, 1, -1);
    }
    send_welco(socket_client, game_play);
    pthread_mutex_lock(&game_play->lock);
    position_player(game_play->labyrinth, player);
    pthread_mutex_unlock(&game_play->lock);
    send_posit(socket_client, player);
    
    while(strcmp(message, "IQUIT***")){
        recu = 0;
        memset(message, 0, sizeof(char)*1024);
        while(recu<250 && strncmp(message+recu-3, "***", 3)!=0){
            recu += recv(socket_client, message+recu, 250-recu, 0);
        }
        if(recu<=0){
            perror("error recv");
            return NULL;
        }
        pthread_mutex_lock(&game_play->lock);
        int nb_ghosts = game_play->labyrinth->nb_ghosts;
        pthread_mutex_unlock(&game_play->lock);
        if(VERB){
            printf("[ %s ]\n", message);
        }
        if(nb_ghosts==0){
            if(send(socket_client, "GOBYE***",8,0)==-1){
                perror("error send");
            }
            close(socket_client);
            return NULL;
        }
        message[recu] = '\0';
        if(strncmp(message, "UPMOV", 5)==0 || strncmp(message, "RIMOV", 5)==0 || strncmp(message, "LEMOV", 5)==0 || strncmp(message, "DOMOV", 5)==0) {
            if(check_mov(message, recu)==0){
                goto error;
            }
            if(send_move(game_play,player,socket_client,message)==-1){
                perror("error send");
                return NULL;
            }
        }else if(strncmp(message, "SEND?", 5)==0){
            if(check_send(message, recu)==0){
                goto error;
            }
            char id[9];
            strncpy(id, message+6, 8);
            id[8] ='\0';
            pthread_mutex_lock(&game_play->lock);
            struct player * pl = get_player(game_play->players, id);
            pthread_mutex_unlock(&game_play->lock);
            if(pl==NULL){
                if(send(socket_client,"NSEND***", 8, 0)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                message[recu-3] = '\0';
                if(send_messp(pl->ip, pl->port, player->id, message+15)==-1){
                    if(send(socket_client,"NSEND***",8,0)==-1){
                        perror("error send");
                        return NULL;
                    }
                }
                else{
                    if(send(socket_client,"SEND!***",8,0)==-1){
                        perror("error send");
                        return NULL;
                    }

                }
            }
        }else if(strncmp(message, "GLIS?***",8)==0){
           if(check_glis(message, recu)==0){
                goto error;
            }
            if(send_list_player_in_game(game_play,socket_client)==-1){
                perror("error send");
                return NULL;
            }
         }
        else if(strncmp(message, "MALL?", 5)==0){
            if(check_mall(message, recu)==0){
                goto error;
            }
            if(message[5]!=' ' || strcmp(message+recu-3, "***")!=0){
                perror("error Protocol");
                close(socket_client);
                return NULL;
            }
            message[recu-3] = '\0';
            if(send_messa(game_play->ip, game_play->port, player->id, message+6)==-1){
                close(socket_client);
                return NULL;
            }else{
                if(send(socket_client,"MALL!***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
        }else if(strcmp(message, "ITEM?***")==0){
            if(check_item(message, recu)==0){
                goto error;
            }
            if(game_play->items){
                if(send_item(player, socket_client, game_play->lock)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                if(send(socket_client,"ITOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
        }else if(strncmp(message, "ITTEL", 5)==0){
            if(check_ittel(message, recu)==0){
                goto error;
            }
            if(game_play->items){
                if(send_ittel(socket_client, player, game_play, message)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                if(send(socket_client,"ITOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
        }else if(strcmp(message, "ITBON***")==0){
            if(check_itbon(message, recu)==0){
                goto error;
            }
            if(game_play->items){
                if(send_itbon(socket_client, player)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                if(send(socket_client,"ITOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
        }else if(strncmp(message, "ITTRA", 5)==0){
            if(check_ittra(message, recu)==0){
                goto error;
            }
            if(game_play->items){
                if(send_ittra(socket_client, player, game_play, message)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                if(send(socket_client,"ITOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
        }else if(strncmp(message, "KILLG", 5)==0){
            if(check_killg(message, recu)==0){
                goto error;
            }
            if(player->cheat){
                if(send_killg(game_play, player,socket_client)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                if(send(socket_client,"CHOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
        }else if(strncmp(message, "LABSH", 5)==0){
            if(check_labsh(message, recu)==0){
                goto error;
            }
            pthread_mutex_lock(&game_play->lock);
            if(player->cheat){
                pthread_mutex_unlock(&game_play->lock);
                if(send_labsh(game_play,socket_client)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"CHOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
        }else if(strncmp(message, "GODME", 5)==0){
            if(check_godme(message, recu)==0){
                goto error;
            }
            pthread_mutex_lock(&game_play->lock);
            if(player->cheat && player->godmode==0){
                player->godmode = 1;
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"GODON***",8,0)==-1){
                    perror("error send");
                    
                    return NULL;
                    
                }
            }else if(player->cheat && player->godmode){
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"ACTIF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"CHOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
        }else if(strncmp(message, "UNGOD", 5)==0){
            if(check_ungod(message, recu)==0){
                goto error;
            }
            pthread_mutex_lock(&game_play->lock);
            if(player->cheat && player->godmode){
                player->godmode = 0;
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"GDOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }else if(player->cheat && player->godmode==0){
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"NOTON***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"CHOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
        }else if(strncmp(message, "GIVCH", 5)==0){
            if(check_givch(message, recu)==0){
                goto error;
            }
            char id[9];
            strncpy(id, message+6, 8);
            id[8] ='\0';
            pthread_mutex_lock(&game_play->lock);
            struct player * reciever = get_player(game_play->players, id);
            pthread_mutex_unlock(&game_play->lock);
            if(reciever != NULL){
                pthread_mutex_lock(&game_play->lock);
                if(player->cheat && reciever->cheat==0){
                    pthread_mutex_unlock(&game_play->lock);
                    if(send_givch(socket_client, player, reciever)==-1){
                        perror("error send");
                        return NULL;
                    }
                }else if(player->cheat && reciever->cheat){
                    pthread_mutex_unlock(&game_play->lock);
                    if(send(socket_client,"HASCH***",8,0)==-1){
                        perror("error send");
                        return NULL;
                    }
                }else{
                    pthread_mutex_unlock(&game_play->lock);
                    if(send(socket_client,"CHOFF***",8,0)==-1){
                        perror("error send");
                        return NULL;
                    }
                }
            }else{
                pthread_mutex_lock(&game_play->lock);
                if(player->cheat){
                    pthread_mutex_unlock(&game_play->lock);
                    if(send(socket_client,"NOPLY***",8,0)==-1){
                        perror("error send");
                        return NULL;
                    }
                }else{
                    pthread_mutex_unlock(&game_play->lock);
                    if(send(socket_client,"CHOFF***",8,0)==-1){
                        perror("error send");
                        return NULL;
                    }
                }
            }
        }else if(strncmp(message, "ITINF", 5)==0){
            if(check_itinf(message, recu)==0){
                goto error;
            }
            pthread_mutex_lock(&game_play->lock);
            if(player->cheat && player->itemInf == 0 && game_play->items){
                player->itemInf = 1;
                if(player->items[0] == 0){
                    player->items[0] = 1;
                }
                if(player->items[1] == 0){
                    player->items[1] = 1;
                }
                if(player->items[2] == 0){
                    player->items[2] = 1;
                }
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"INFON***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }else if(player->cheat && player->itemInf && game_play->items){
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"ACTIF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }else if(player->cheat == 0 && game_play->items){
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"CHOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"ITOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
        }else if(strncmp(message, "NOINF", 5)==0){
            if(check_noinf(message, recu)==0){
                goto error;
            }
            pthread_mutex_lock(&game_play->lock);
            if(player->cheat && player->itemInf && game_play->items){
                player->itemInf = 0;
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"INOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }else if(player->cheat && player->itemInf == 0 && game_play->items){
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"NOTON***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }else if(player->cheat == 0 && game_play->items){
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"CHOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }else{
                pthread_mutex_unlock(&game_play->lock);
                if(send(socket_client,"ITOFF***",8,0)==-1){
                    perror("error send");
                    return NULL;
                }
            }
        }else if(strncmp(message, "IQUIT", 5)==0){
            if(check_iquit(message, recu)==0){
                goto error;
            }
            if(send_gobye(socket_client,player,game_play)==-1){
                perror("error send");
                return NULL;
            }
            break;
        }else{
            printf("Message reçu non conforme : %s\n", message);
            close(socket_client);
            return NULL;
        }
    }
    return NULL;
    error:
        printf("Message reçu non conforme : %s\n", message);
        close(socket_client);
        return NULL;
    
    
}

void sig_handler(int signum){
    free_list_games(games_no_start);
    free_list_games(games_start);
    signal(SIGINT,SIG_DFL);
    kill(getpid(), SIGINT);
}

int main(int argc, char * argv[]) {
    signal(SIGINT,sig_handler);
    if(argc!=3){
        printf("Veuillez donner un numéro de port en argument ainsi que 0 ou 1 pour le mode verbeux\n");
        return -1;
    }
    if(strncmp(argv[2],"0",1) == 0){
        VERB = 0;
    }else if(strncmp(argv[2],"1",1) == 0){
        VERB = 1;
    }else{
        printf("L'argument pour le mode verbeux doit soit être 0 ou 1.\n");
        return -1;
    }
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if(s==-1){
        perror("error socket");
        return -1;
    }
    ip_multicast.s_addr = inet_addr("225.1.2.4");
    games_no_start = create_list_game();
    games_start = create_list_game();
    strncpy(port, argv[1], 4);
    port[4] = '\0'; 
    struct sockaddr_in adresse;
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(atoi(argv[1]));
    adresse.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(s,(struct sockaddr *)&adresse, sizeof(struct sockaddr_in))==0){
        if(listen(s, 0)==0){
            socklen_t s_len = sizeof(struct sockaddr_in);
            while(1){
                int socket_client = accept(s, (struct sockaddr *)&adresse, &s_len);
                if(socket_client==-1){
                    perror("error accept");
                    continue;
                }
                char *info = malloc(sizeof(int) + sizeof(adresse.sin_addr));
                memcpy(info, &socket_client, sizeof(int));
                memcpy(info+sizeof(int), &adresse.sin_addr, sizeof(adresse.sin_addr));
                pthread_t th;
                pthread_create(&th,NULL , connexion, info);
            }
        }
        else{
            perror("error listen");
            return -1;
        }
    }
    else{
        perror("error bind");
        return -1;
    }
    return 0;
}