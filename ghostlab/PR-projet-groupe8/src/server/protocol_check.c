
#include "../../include/protocol_check.h"
int is_numeric(char *message, int length){
    for(int i=0; i<length; i++){
        if((message[i]>= '0' && message[i] <= '9')){
           continue;
        }
        return 0;
    }
    return 1;
}

int is_alpha_numeric(char *message, int length){
   for(int i=0; i<length; i++){
        if((message[i]>= '0' && message[i] <= '9') || (message[i]>='A' && message[i] <= 'Z') || (message[i]>='a' && message[i] <= 'z')){
            continue;
        }
        return 0;
    }
    return 1;
}
int check_start(char * message, int length){
    if(length==8 && strncmp(message, "START***", 8)==0){
        return 1;
    }
    return 0;
}

int check_newpl(char * message, int length){
    if(length==22 && message[5]==' ' && message[14]==' ' && strncmp(message, "NEWPL", 5)==0 && strncmp(message+length-3, "***", 3)==0){
        return 1;
    }
    return 0;
}


int check_regis(char * message, int length){
    if(length==24 && message[5]==' ' && message[14]==' ' && message[19]==' ' && strncmp(message, "REGIS", 5)==0 && strncmp(message+length-3, "***", 3)==0){
        return 1;
    }
    return 0;
}

int check_game(char * message, int length){
    if(length==8 && strncmp(message, "GAME?***", 8)==0){
        return 1;
    }
    return 0;
}


int check_list(char * message, int length){
    if(length==10 && message[5]==' ' && strncmp(message, "LIST?", 5)==0 && strncmp(message+length-3, "***", 3)==0){
        return 1;
    }
    return 0;
}

int check_size(char * message, int length){
    if(length==10 && message[5]==' ' && strncmp(message, "SIZE?", 5)==0 && strncmp(message+length-3, "***", 3)==0){
        return 1;
    }
    return 0;
}

int check_unreg(char * message, int length){
    if(length==8 && strncmp(message, "UNREG***", 8)==0){
        return 1;
    }
    return 0;
}

int check_itmon(char * message, int length){
    if(length==8 && strncmp(message, "ITMON***", 8)==0){
        return 1;
    }
    return 0;
}

int check_cheat(char * message, int length){
    if(length==8 && strncmp(message, "CHEAT***", 8)==0){
        return 1;
    }
    return 0;
}



int check_iquit(char * message, int length){
    if(length==8 && strncmp(message, "IQUIT***", 8)==0){
        return 1;
    }
    return 0;
}

int check_mov(char * message, int length){
    if(is_numeric(message+6, 3) && length==12 && message[5]==' ' && (strncmp(message, "UPMOV", 5)==0 || strncmp(message, "RIMOV", 5)==0 || strncmp(message, "LEMOV", 5)==0 
        || strncmp(message, "DOMOV", 5)==0) && strncmp(message+length-3, "***", 3)==0){
        return 1;
    }
    return 0;
}

int check_send(char * message, int length){
    if(length<=218 && length>17 && message[5]==' ' && message[14]==' ' && strncmp(message, "SEND?", 5)==0 && strncmp(message+length-3, "***", 3)==0){
        return 1;
    }
    return 0;
}

int check_glis(char * message, int length){
    if(length==8 && strncmp(message, "GLIS?***", 8)==0){
        return 1;
    }
    return 0;
}

int check_mall(char * message, int length){
    if(length<=209 && length>8 && message[5]==' ' && strncmp(message, "MALL?", 5)==0 && strncmp(message+length-3, "***", 3)==0){
        return 1;
    }
    return 0;
}

int check_item(char * message, int length){
    if(length==8 && strncmp(message, "ITEM?***", 8)==0){
        return 1;
    }
    return 0;
}

int check_ittel(char * message, int length){
    if(length==16 && message[5]==' ' && message[9]==' ' && strncmp(message, "ITTEL", 5)==0 && strncmp(message+length-3, "***", 5)==0){
        return 1;
    }
    return 0;
}

int check_itbon(char * message, int length){
    if(length==8 && strncmp(message, "ITBON***", 8)==0){
        return 1;
    }
    return 0;
}

int check_ittra(char * message, int length){
    if(length==16 && message[5]==' ' && message[9]==' ' && strncmp(message, "ITTRA", 5)==0 && strncmp(message+length-3, "***", 5)==0){
        return 1;
    }
    return 0;
}

int check_killg(char * message, int length){
    if(length==8 && strncmp(message, "KILLG***", 8)==0){
        return 1;
    }
    return 0;
}

int check_labsh(char * message, int length){
    if(length==8 && strncmp(message, "LABSH***", 8)==0){
        return 1;
    }
    return 0;
}

int check_godme(char * message, int length){
    if(length==8 && strncmp(message, "GODME***", 8)==0){
        return 1;
    }
    return 0;
}

int check_ungod(char * message, int length){
    if(length==8 && strncmp(message, "UNGOD***", 8)==0){
        return 1;
    }
    return 0;
}

int check_givch(char * message, int length){
    if(length==17 && message[5]==' ' && strncmp(message, "GIVCH", 5)==0 && strncmp(message+length-3, "***", 5)==0 ){
        return 1;
    }
    return 0;
}

int check_itinf(char * message, int length){
    if(length==8 && strncmp(message, "ITINF***", 8)==0){
        return 1;
    }
    return 0;
}

int check_noinf(char * message, int length){
    if(length==8 && strncmp(message, "NOINF***", 8)==0){
        return 1;
    }
    return 0;
}

int check_resiz(char * message, int length){
    if(length==16 && message[5]==' ' && message[8]==' ' && message[11]==' ' && strncmp(message, "RESIZ", 5)==0 && strncmp(message+length-3, "***", 5)==0){
        return 1;
    }
    return 0;
}