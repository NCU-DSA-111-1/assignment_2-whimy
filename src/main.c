// Open new game : –n –s new_game_file_name
// Load game : –l old_game_file_name
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "backprop.h"
#include "board.h"
#include "goma.h"
#include "file.h"

int para;
FILE *fp = NULL;
BOARD **ptboard = NULL;
char *condition = NULL;

int main(int argc, char *argv[]) {
    // detect which program to execute
    while((para = getopt(argc, argv, "nsl:")) != -1) 
    { 
        switch(para) 
        {
            // Open new game
            case 'n':
                printf("[Open new game]\n");
                Open_new_game(fp,argv[3]);
                game_start();
                break;
            case 'l':
                printf("[Reload old game]\n");
                Reload_old_game(fp,argv[2]);
                // read_board
                break;
            case '?':
                printf("Unknown parameter.\n");
                printf("Please try again.\n");
                exit(EXIT_FAILURE);
        } 
    } 
    printf("Successfully execute!\nExit program.");
    return 0;
}

void game_start(){
    char result = NA;
    int turns = 1;
    int player = X;

    // init_board
    printf("Initialize board...\n\n");
    ptboard = (BOARD**)malloc(COLUMN * sizeof(BOARD*));
    for(int r = 0;r < ROW;r++){
        ptboard[r] = (BOARD*)malloc(ROW * sizeof(BOARD)); 
    }
    // init chess
    printf("Generate goma...\n\n");
    generate_goma();

    // init bag
    printf("Initialize goma bag...\n\n");
    x_bag = init_bag(x_bag);
    y_bag = init_bag(y_bag);


    // initialize board
    printf("Initialize board...\n\n");
    init_board(ptboard);
    visualize_board(ptboard);

    do{
        printf("[Turn%d]\n",turns);
        printf("[Player %c]\n",(player)? 'y':'x');
        goma_move(player);
        visualize_board(ptboard);
        player = !player;
        turns++;
    }while(result == NA);
    
    if(result == 'x'){
        printf("player x wins!\n");
    }else{
        printf("Player y wins!\n");
    }
    printf("\nClosing game...\n\n");

}

void goma_move(int user){
    int *ptx = (int*)malloc(2*sizeof(int));
    int *pty = (int*)malloc(2*sizeof(int));
    BOARD *pre_select = (BOARD*)malloc(sizeof(BOARD));
    BOARD *new_select = (BOARD*)malloc(sizeof(BOARD));
    condition = (char*)malloc(sizeof(char));
    // feed init_pos
    do{
        do{   
            printf("Please enter the initial position:");
            scanf("%d",ptx);
            scanf("%d",pty);
            pre_select = &ptboard[*pty - 1][ROW - *ptx];
                 
            if(*ptx <= 0 || *pty <= 0 || *ptx > ROW || *pty > COLUMN){
                printf("\n\033[31m[Error 0]:This position does not exist.\033[m\nPlease try again.\n\n");
            }
            else if(pre_select->goma == NULL){
                printf("\n\033[31m[Error 1]:There is empty.\033[m\nPlease try again.\n\n");
            }
            else if(pre_select->owner != ((user)? 'y':'x')){
                printf("\n\033[31m[Error 2]This is not your chess.\033[m\nPlease try again.\n\n");
            }else{
                printf("Select ");
                if(pre_select->owner == 'x'){
                    print_xgoma(pre_select->goma->syb);
                }else{
                    print_ygoma(pre_select->goma->syb);
                }
                printf("[%d][%d]",*ptx,*pty);
                printf("\n\n");
                break;
            }
        }while(TRUE);

        // feed new position
        do{   
            printf("Please enter the new position:");
            scanf("%d",ptx+1);
            scanf("%d",pty+1);
            new_select = &ptboard[*(pty+1) - 1][ROW - *(ptx+1)];

            if(*(ptx+1) <= 0 || *(pty+1) <= 0 || *(ptx+1) > ROW || *(pty+1) > COLUMN){
                printf("\n\033[31m[Error 0]:This position does not exist.\033[m\nPlease try again.\n\n");
            }
            else if(new_select->owner == pre_select->owner){
                printf("\n\033[31m[Error 3]You cannot eat your own chess.\033[m\nPlease try again.\n\n");
            }
            // else if((pre_select->chess->action(user,(int8_t)(*(x+1) - *x), (int8_t)(*(y+1) - *y)))){
            //     continue;
            // }else if(detect_path(pre_select,x, y)){
            //     printf("\nPath Error.");
            //     continue;
            // }
            else{
                printf("Select ");
                if(new_select->owner == 'x'){
                    print_xgoma(new_select->goma->syb);
                }else if(new_select->owner == 'y'){
                    print_ygoma(new_select->goma->syb);
                }else{
                    printf("空 ");
                }
                printf("[%d][%d]", *(ptx+1), *(pty+1));
                printf("\n\n");
                break;
            }
        }while(TRUE);

        if(pre_select->owner == 'x'){
            print_xgoma(pre_select->goma->syb);
        }else{
            print_ygoma(pre_select->goma->syb);
        }
        printf("[%d][%d]->[%d][%d]?\n",*ptx, *pty, *(ptx+1), *(pty+1));

        printf("\nWould you like to re-select?[y/n]:");
        scanf(" %c",condition);
        if(*condition == 'y'){
            printf("\n");
            continue;
        }
        print_step(pre_select,*ptx,*pty);
        break;
    }while(pre_select == NULL || new_select == NULL || *condition == 'y');
    swap(pre_select,new_select);
}



void free_mem(){
    free(ptboard);
    free(PAWN);
    free(LANCE);
    free(KNIGHT);
    free(SLIVER);
    free(GOLD);
    free(KING);
    free(ROOK);
    free(BISHOP);
}

void print_step(BOARD* sel, int x, int y){
    printf("============================\n");
    char *output;
    if(y == 1){
        output = "一";
    }else if(y == 2){
        output = "二";
    }else if(y == 3){
        output = "三";
    }else if(y == 4){
        output = "四";
    }else if(y == 5){
        output = "五";
    }else if(y == 6){
        output = "六";
    }else if(y == 7){
        output = "七";
    }else if(y == 8){
        output = "八";
    }else if(y == 9){
        output = "九";
    }
    if(sel->owner == 'x'){
        print_xgoma("▲");
        printf("\033[31m%d%s\033[m",x,output);
        print_xgoma(sel->goma->syb);
        printf("\n");
    }else{
        print_ygoma("△");
        printf("\033[34m%d\033[m ",x);
        print_ygoma(sel->goma->syb);
    } 
    printf("============================\n");
}
    