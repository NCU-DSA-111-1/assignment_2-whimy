#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <io.h>
#include "file.h"

    // open new game
void Open_new_game(FILE* fp,char *fname){
    printf("Creating file...\n\n");
    strcat(fname,".csv");
    // if(!access(fname,0)){
    //     printf("This file name have already exist.\nWould you like to reload old game_file?\n");
    //     printf("You could reload old game_file by entering $–l {$old_game_file_name}\n");
    //     exit(EXIT_FAILURE);
    // }

    if(!(fp = fopen(fname,"w+"))){
        printf("Fail to open file.\n");
    }else{
        printf("Open file successfully.\n");
    }
    fclose(fp);
}

void Reload_old_game(FILE* fp,char *fname){
    printf("Reloading...\n\n");
    strcat(fname,".csv");
    if(access(fname,0)){
        printf("This file does not exist.\nWould you like to open a new game?\n");
        printf("You could open a new game by entering $./shogi –n –s {$new_game_file_name}\n");
        exit(EXIT_FAILURE);
    }

    if(!(fp = fopen(fname,"r"))){
        printf("Fail to open file.\n");
    }else{
        printf("Open file successfully.\n");
    }
    fclose(fp);
}