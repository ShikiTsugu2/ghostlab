#ifndef PROTOCOL_CHECK_H
#define PROTOCOL_CHECK_H
#include <string.h>
#include <stdio.h>

int check_start(char * message, int length);
int check_newpl(char * message, int length);
int check_regis(char * message, int length);
int check_game(char * message, int length);
int check_list(char * message, int length);
int check_size(char * message, int length);
int check_unreg(char * message, int length);
int check_itmon(char * message, int length);
int check_cheat(char * message, int length);
int check_start(char * message, int length);
int check_iquit(char * message, int length);
int check_mov(char * message, int length);
int check_send(char * message, int length);
int check_glis(char * message, int length);
int check_mall(char * message, int length);
int check_item(char * message, int length);
int check_ittel(char * message, int length);
int check_itbon(char * message, int length);
int check_ittra(char * message, int length);
int check_killg(char * message, int length);
int check_labsh(char * message, int length);
int check_godme(char * message, int length);
int check_ungod(char * message, int length);
int check_givch(char * message, int length);
int check_itinf(char * message, int length);
int check_noinf(char * message, int length);
int is_alpha_numeric(char *message, int length);
int is_numeric(char *message, int length);
int check_resiz(char * message, int length);
#endif // PROTOCOL_CHECK