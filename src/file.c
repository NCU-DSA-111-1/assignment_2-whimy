#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "file.h"

// open new game
void Open_new_game(FILE* fp,char *fname){
    char c;
    printf("Creating file...\n\n");
    strcat(fname,".csv");
    if(!access(fname,0)){
        printf("This file name have already exist.\n");
        do{
            printf("Would you like to rewrite this old game_file?[y/n]:");
            if((c = getchar()) == 'n'){
                printf("You could reload old game_file by entering $–l {$old_game_file_name}\n");
                exit(EXIT_FAILURE);
            }else if(c == 'y'){
                printf("Opening file...\n\n");
                if(!(fp = fopen(fname,"w+"))){
                    printf("Fail to open file.\n");
                    exit(EXIT_FAILURE);
                }else{
                    printf("Open file successfully.\n");
                    
                }
                fprintf(fp, "先後手,筋,段,駒名,\n");
                fclose(fp);
                break;
            }else{
                printf("Please enter 'y' or 'n'.\n");
                getchar();
            }
        }while(c != 'y' && c != 'n');
    }
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

NODE* Add_to_list(NODE* head, int *px, int *py){
    NODE *new;

    if(head == NULL){
        head = (NODE*)malloc(sizeof(NODE));
        head->cx = px;  head->cy = py;
        head->old_pos = ptboard[*py-1][ROW-*px];
        head->new_pos = ptboard[*(py+1)-1][ROW - *(px+1)];
        head->next = NULL;
    }else{
        new = (NODE*)malloc(sizeof(NODE));
        new->cx = px;  new->cy = py;
        new->old_pos = ptboard[*py-1][ROW-*px];
        new->new_pos = ptboard[*(py+1)-1][ROW - *(px+1)];
        new->next = head;
        head = new;
    }
    return head;
}



void write_file(FILE* fp, char *fname, int* px, int* py){
    // 寫入先後手
    fp = fopen(fname,"a+");
    if(ptboard[*py-1][ROW - *px].owner == 'x'){
        fprintf(fp,"先手▲,");
    }else if(ptboard[*py-1][ROW - *px].owner == 'y'){
        fprintf(fp,"後手△,");
    }
    // 以羅馬數字寫入筋
    fprintf(fp, "%d,",*(px+1));
    // 以國字寫入段
    if(*(py+1) == 1){
        fprintf(fp,"一,");
    }else if(*(py+1) == 2){
        fprintf(fp,"二,");
    }else if(*(py+1) == 3){
        fprintf(fp,"三,");
    }else if(*(py+1) == 4){
        fprintf(fp,"四,");
    }else if(*(py+1) == 5){
        fprintf(fp,"五,");
    }else if(*(py+1) == 6){
        fprintf(fp,"六,");
    }else if(*(py+1) == 7){
        fprintf(fp,"七,");
    }else if(*(py+1) == 8){
        fprintf(fp,"八,");
    }else if(*(py+1) == 9){
        fprintf(fp,"九,");
    }
    // 以國字寫入棋
    if(ptboard[*py-1][ROW -*px].goma == Pawn){
        fprintf(fp, PAWN);
    }else if(ptboard[*py-1][ROW -*px].goma == Lance){
        fprintf(fp, LANCE);
    }else if(ptboard[*py-1][ROW -*px].goma == Knight){
        fprintf(fp, KNIGHT);
    }else if(ptboard[*py-1][ROW -*px].goma == Sliver){
        fprintf(fp, SLIVER);
    }else if(ptboard[*py-1][ROW -*px].goma == Gold){
        fprintf(fp, GOLD);
    }else if(ptboard[*py-1][ROW -*px].goma == King){
        fprintf(fp, KING);
    }else if(ptboard[*py-1][ROW -*px].goma == Rook){
        fprintf(fp, ROOK);
    }else if(ptboard[*py-1][ROW -*px].goma == Bishop){
        fprintf(fp, BISHOP);
    }
    fprintf(fp,"\n");
    fclose(fp);
}
