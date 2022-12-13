#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "board.h"
#include "goma.h"

// initialize board
void init_board(BOARD** ptb){
    // 後手
    ptb[0][0].goma = Lance;    ptb[0][8].goma = Lance;
    ptb[0][1].goma = Knight;   ptb[0][7].goma = Knight;
    ptb[0][2].goma = Sliver;   ptb[0][6].goma = Sliver;
    ptb[0][3].goma = Gold;     ptb[0][5].goma = Gold;
    ptb[0][4].goma = King;
    for(int r = 0;r  < ROW;r++){
        ptb[0][r].owner = 'y';
    }

    for(int r = 0;r < ROW;r++){
        if(r == 1){
            ptb[1][r].goma = Rook;
            ptb[1][r].owner = 'y';
        }else if(r == 7){
            ptb[1][r].goma = Bishop;
            ptb[1][r].owner = 'y';
        }else{
            ptb[1][r].goma = NULL;
            ptb[1][r].owner = NA;
        }
    }
        
    for(int r = 0; r < ROW;r++){
        ptb[2][r].goma = Pawn;
        ptb[2][r].owner = 'y';
    }

    for(int c = 3; c < 6;c++){
        for(int r = 0;r < ROW;r++){
            ptb[c][r].goma = NULL;
            ptb[c][r].owner = NA;
        }
    }

    // 先手
    ptb[8][0].goma = Lance;    ptb[8][8].goma = Lance;
    ptb[8][1].goma = Knight;   ptb[8][7].goma = Knight;
    ptb[8][2].goma = Sliver;   ptb[8][6].goma = Sliver;
    ptb[8][3].goma = Gold;     ptb[8][5].goma = Gold;
    ptb[8][4].goma = King;
    for(int r = 0;r  < ROW;r++){
        ptb[8][r].owner = 'x';
    }

    for(int r = 0;r < ROW;r++){
        if(r == 1){
            ptb[7][r].goma = Bishop;
            ptb[7][r].owner = 'x';
        }else if(r == 7){
            ptb[7][r].goma = Rook;
            ptb[7][r].owner = 'x';
        }else{
            ptb[7][r].goma = NULL;
            ptb[7][r].owner = NA;
        }
    }

    for(int r = 0;r < ROW;r++){
        ptb[6][r].goma = Pawn;
        ptb[6][r].owner = 'x';
    } 

    for(int c = 0;c<COLUMN;c++){
        for(int r = 0;r<ROW;r++)
        ptb[c][r].number = c*9+r;
    }
}

void visualize_board(BOARD** const ptb){
    // Show goma bag
    printf("=========== y駒台 ==========\n");
    for(int num = 0;num <= y_bag->top;num++){
        print_xgoma(y_bag->mochi[num].syb);
    }
    printf("\n============================\n");
    // row header
    for(int z = 0; z < ROW;z++){
            printf("%1d  ",ROW-z);
    }
    printf("\n");

    for(int c = 0;c < COLUMN;c++){
        for(int r = 0;r< ROW;r++){
            if(ptb[c][r].owner == 'x'){
                print_xgoma(ptb[c][r].goma->syb);
            }else if(ptb[c][r].owner == 'y'){
                print_ygoma(ptb[c][r].goma->syb);
            }else{
                printf("%s ",EMPTY);
            }
        }
        // column header
        printf("%d\n",c+1);
    }
    printf("============================\n");
    for(int num = 0;num <= x_bag->top;num++){
        print_ygoma(x_bag->mochi[num].syb);
    }
    printf("\n========== x駒台 ===========\n");

}

void swap(BOARD* origin, BOARD *goal){
    printf("Swaping...\n\n");
    BOARD *temp = malloc(sizeof(BOARD));
    if(goal->goma != NULL){
        *temp = *goal;
        if(origin->owner == 'x'){
            x_bag->top++;
            x_bag->mochi[x_bag->top] = *(temp->goma);
        }else{
            y_bag->top++;
            y_bag->mochi[y_bag->top] = *(temp->goma);
        }
        *goal = *origin;
        origin->goma = NULL;
        origin->owner = NA;

    }else{
        *temp = *goal;
        *goal = *origin;
        *origin = *temp;
    }
}

