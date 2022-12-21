#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "file.h"

char buffer[10];

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
    fp = fopen(fname,"r+");
    read_file(fp, fname);
    fclose(fp);
}

NODE* Add_to_list(NODE* head, int *px, int *py){
    NODE *new;

    if(head == NULL){
        head = (NODE*)malloc(sizeof(NODE));
        head->cx[0] = *px;  head->cy[0] = *py;
        head->cx[1] = *(px+1);  head->cy[1] = *(py+1);
        head->old_pos = ptboard[*py-1][ROW-*px];
        head->new_pos = ptboard[*(py+1)-1][ROW - *(px+1)];
        head->next = NULL;
    }else{
        new = (NODE*)malloc(sizeof(NODE));
        new->cx[0] = *px;  new->cy[0] = *py;
        new->cx[1] = *(px+1);  new->cy[1] = *(py+1);
        new->old_pos = ptboard[*py-1][ROW-*px];
        new->new_pos = ptboard[*(py+1)-1][ROW - *(px+1)];
        new->next = head;
        head = new;
    }

    return head;
}

NODE* Remove_from_list(NODE* head, BAG *ptbag){
    NODE *rel;
    BOARD *temp = malloc(sizeof(BOARD));
    rel = (NODE*)malloc(sizeof(NODE));
    rel->cx[0] = head->cx[0];     rel->cy[0] = head->cy[0];
    rel->cx[1] = head->cx[1];     rel->cy[1] = head->cy[1];
    rel->new_pos = head->new_pos;   rel->old_pos = head->old_pos;
    rel->next = head->next;
    head = rel->next;
    // printf("%s move from [%d][%d] to [%d][%d]\n", rel->old_pos.goma->syb, *(rel->cx), *(rel->cy), *(rel->cx+1), *(rel->cy+1));
    if(rel->new_pos.goma != NULL){
        ptboard[*(rel->cy+1) - 1][ROW - *(rel->cx+1)] = rel->new_pos;
        ptboard[*(rel->cy) - 1][ROW - *(rel->cx)] = rel->old_pos;
        ptbag->mochi[ptbag->top] = *Empty;
        ptbag->top--;
    }else{
        *temp = ptboard[*(rel->cy+1) - 1][ROW - *(rel->cx+1)];
        ptboard[*(rel->cy+1) - 1][ROW - *(rel->cx+1)] = ptboard[*(rel->cy) - 1][ROW - *(rel->cx)];
        ptboard[*(rel->cy) - 1][ROW - *(rel->cx)] = *temp;
    }


    free(rel);
    return head;
}

void write_file(FILE* fp, char *fname, NODE *head, int* px, int* py){
    // 寫入先後手
    fp = fopen(fname,"a+");

    if(head->old_pos.goma ==  Pawn){
        fprintf(fp,"%-3c", 'p');
    }else if(head->old_pos.goma ==  Lance){
        fprintf(fp,"%-3c", 'l');
    }else if(head->old_pos.goma ==  Knight){
        fprintf(fp,"%-3c", 'n');
    }else if(head->old_pos.goma ==  Sliver){
        fprintf(fp,"%-3c", 's');
    }else if(head->old_pos.goma ==  Gold){
        fprintf(fp,"%-3c", 'g');
    }else if(head->old_pos.goma ==  King){
        fprintf(fp,"%-3c", 'k');
    }else if(head->old_pos.goma ==  Rook){
        fprintf(fp,"%-3c", 'r');
    }else if(head->old_pos.goma ==  Bishop){
        fprintf(fp,"%-3c", 'b');
    }else{
        fprintf(fp,"%-3c", 'x');
    }
    
    fprintf(fp, "%-3d%-3d", head->cx[0], head->cy[0]);

    if(head->new_pos.goma ==  Pawn){
        fprintf(fp,"%-3c", 'p');
    }else if(head->new_pos.goma ==  Lance){
        fprintf(fp,"%-3c", 'l');
    }else if(head->new_pos.goma ==  Knight){
        fprintf(fp,"%-3c", 'n');
    }else if(head->new_pos.goma ==  Sliver){
        fprintf(fp,"%-3c", 's');
    }else if(head->new_pos.goma ==  Gold){
        fprintf(fp,"%-3c", 'g');
    }else if(head->new_pos.goma ==  King){
        fprintf(fp,"%-3c", 'k');
    }else if(head->new_pos.goma ==  Rook){
        fprintf(fp,"%-3c", 'r');
    }else if(head->new_pos.goma ==  Bishop){
        fprintf(fp,"%-3c", 'b');
    }else{
        fprintf(fp,"%-3c", 'x');
    }

    fprintf(fp, "%-3d%-3d", head->cx[1], head->cy[1]);
    fprintf(fp,"\n");
    fclose(fp);
}


void read_file(FILE* fp, char *fname){
    typedef struct buffer{
        char bgoma[2];
        int bx[2];
        int by[2];
    }BUFFER;
    NODE *flist = NULL;
    BUFFER *buf;
    BOARD *pre_select = (BOARD*)malloc(sizeof(BOARD));
    BOARD *new_select = (BOARD*)malloc(sizeof(BOARD));
    char fb;
    int turn = 0;

    printf("Building the old structure...\n\n");
    ptboard = (BOARD**)malloc(COLUMN * sizeof(BOARD*));
    for(int r = 0;r < ROW;r++){
        ptboard[r] = (BOARD*)malloc(ROW * sizeof(BOARD)); 
    }
    generate_goma();
    x_bag = init_bag(x_bag);
    y_bag = init_bag(y_bag);
    init_board(ptboard);
    visualize_board(ptboard);
    
    do{
        buf = (BUFFER*)malloc(sizeof(BUFFER));
        printf("Turn[%d]\n",turn);
        printf("\nPress 'f' for the next step, 'b' for the back step:");
        scanf(" %c", &fb);
        // printf("%c %d %d %c %d %d\n", (buf->bgoma[0]), (buf->bx[0]), (buf->by[0]), (buf->bgoma[1]), (buf->bx[1]), (buf->by[1]));
        if(fb == 'f'){
            if(feof(fp)){
                printf("This is already the final step.\n");
                printf("Would you like to exit?[y/n]:");
                scanf(" %c", &fb);
                if(fb == 'y'){
                    break;
                }else{
                    continue;
                }
            }else{
                fscanf(fp, "%c %d %d %c %d %d ", &(buf->bgoma[0]), &(buf->bx[0]), &(buf->by[0]), &(buf->bgoma[1]), &(buf->bx[1]), &(buf->by[1]));
                flist = Add_to_list(flist, buf->bx, buf->by);
                pre_select = &ptboard[buf->by[0] - 1][ROW - buf->bx[0]];
                new_select = &ptboard[buf->by[1] - 1][ROW - buf->bx[1]];
                swap(pre_select,new_select);
                turn++;
            }
        }else if(fb == 'b'){
            if(flist == NULL){
                printf("This is already the first step.\n");
                printf("Would you like to exit?[y/n]:");
                scanf(" %c", &fb);
                if(fb == 'y'){
                    break;
                }else{
                    continue;
                }
            }else{
                fseek(fp, -(sizeof(BUFFER)-1), SEEK_CUR);
                if(turn%2){
                    flist = Remove_from_list(flist, x_bag);
                }else{
                    fseek(fp, -(sizeof(BUFFER)-1), SEEK_CUR);
                    flist = Remove_from_list(flist, y_bag);
                }
                turn--;
            }
        }else{
            printf("Invalid input.\nPlease try again.\n");
            continue;
        }
        free(buf);
        visualize_board(ptboard);
    }while(1);
    printf("This is the end of the file.\n");
}


