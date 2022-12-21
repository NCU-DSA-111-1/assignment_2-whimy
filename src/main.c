// Open new game : –n –s new_game_file_name
// Load game : –l old_game_file_name
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <ev.h>
#include "backprop.h"
#include "board.h"
#include "goma.h"
#include "file.h"

int para;
FILE *fp = NULL;
NODE* list = NULL;
char *condition = NULL;
int regret;
int player = X;
ev_io stdin_watcher;
ev_timer timeout_watcherX;
ev_timer timeout_watcherY;
char *name = NULL;

int main(int argc, char *argv[]) {
    // detect which program to execute
    name = argv[3]; 
    while((para = getopt(argc, argv, "nsl:")) != -1) 
    { 
        switch(para) 
        {
            // Open new game
            case 'n':
                printf("[Open new game]\n");
                Open_new_game(fp,argv[3]);
                game_start(argv[3]);
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

void game_start(char *gname){
    // initial setting
    char result = NA;
    int turns = 1;
    struct ev_loop *loop = EV_DEFAULT;
    ev_io_init(&stdin_watcher, stdin_cb, 0, EV_READ);
    ev_timer_init(&timeout_watcherX,timeout_cbX, 3, 0);
    ev_timer_init(&timeout_watcherY,timeout_cbY, 3, 0);

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

    init_board(ptboard);
    visualize_board(ptboard);

    do{
        ev_io_start(loop, &stdin_watcher);
        printf("[Turn%d]\n",turns);
        printf("[Player %c]\n",(player)? 'y':'x');
        if(!player){
            ev_timer_start(loop, &timeout_watcherX);
        }else{
            ev_timer_start(loop, &timeout_watcherY);
        }
        // regret?
        if(turns > 2){
            printf("Would you like to regret?[0/1]:");
            scanf(" %d", &regret);
            if(!regret){
                printf("Regreting...\n\n");
                list = Remove_from_list(list, (player)? x_bag:y_bag);
                list = Remove_from_list(list, (player)? y_bag:x_bag);
                turns = turns - 2;
                visualize_board(ptboard);
                continue;
            }
        }

        goma_move(gname);
        ev_run(loop, 0);
        visualize_board(ptboard);


        if(list->new_pos.goma == King){
            if(list->new_pos.owner == 'y'){
                printf("Player x win!\n");
            }else{
                printf("Player y win!\n");
            }
        }

        // change player
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

void goma_move(char *gname){
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
                printf("\n\033[31m[Error 1]:This position does not exist.\033[m\nPlease try again.\n\n");
            }
            else if(pre_select->goma == NULL){
                printf("\n\033[31m[Error 1]:There is empty.\033[m\nPlease try again.\n\n");
            }
            else if(pre_select->owner != ((player)? 'y':'x')){
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
            scanf(" %d",ptx+1);
            scanf(" %d",pty+1);
            new_select = &ptboard[*(pty+1) - 1][ROW - *(ptx+1)];

            if(*(ptx+1) <= 0 || *(pty+1) <= 0 || *(ptx+1) > ROW || *(pty+1) > COLUMN){
                printf("\n\033[31m[Error 1]:This position does not exist.\033[m\nPlease try again.\n\n");
                new_select = NULL;
            }
            else if(new_select->owner == pre_select->owner){
                printf("\n\033[31m[Error 2]You cannot eat your own chess.\033[m\nPlease try again.\n\n");
                new_select = NULL;
            }  
            else if(pre_select->goma->act(pre_select->owner, ptx, pty, OFF)){
                new_select = NULL;
            }
            else{
                // detect_path(ptx,pty);
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
            }
        }while(new_select == NULL);

        if(pre_select->owner == 'x'){
            print_xgoma(pre_select->goma->syb);
        }else{
            print_ygoma(pre_select->goma->syb);
        }
        printf("[%d][%d]->[%d][%d]?\n",*ptx, *pty, *(ptx+1), *(pty+1));

        do{
            printf("\nWould you like to re-select?[y/n]:");
            scanf(" %c",condition);
            if(*condition == 'y'){
                printf("\n");
                break;
            }else if(*condition == 'n'){
                printf("\n");
                break;
            }else{
                printf("Please enter 'y' or 'n'.\n");
                *condition = NA;
            }
        }while(*condition == NA);

    }while(pre_select == NULL || new_select == NULL || *condition == 'y');
    list = Add_to_list(list,ptx,pty);
    write_file(fp, gname, list, ptx, pty);
    swap(pre_select,new_select);
    printf("%s move from [%d][%d] to [%d][%d]\n", list->old_pos.goma->syb, list->cx[0], list->cy[0], list->cx[1], list->cy[1]);

    printf("\nWould you want to leave the game?[s/n]");
    scanf(" %c", condition);
    if(*condition == 's' || *condition == 'S'){
        printf("Saving...\n\n");
        printf("Saving completed.\nGoodbye.\n");
        exit(EXIT_SUCCESS);
    }
}

void free_mem(){
    free(ptboard);
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
        printf("\033[31m%d%s\033[m ",x,output);
        print_xgoma(sel->goma->syb);
        printf("\n");
    }else{
        print_ygoma("△");
        printf("\033[34m%d%s\033[m ",x,output);
        print_ygoma(sel->goma->syb);
        printf("\n");
    } 
    printf("============================\n");
}
    

static void timeout_cbX(EV_P_ ev_timer *w, int revents){
    printf("Total time:%d\n",(int)ev_now(loop));
    // ev_break(EV_A_ EVBREAK_ONE);
    ev_timer_stop(loop, &timeout_watcherX);
    ev_timer_start(loop, &timeout_watcherY);
    ev_break(EV_A_ EVBREAK_ONE);
}

static void timeout_cbY(EV_P_ ev_timer *w, int revents){
    printf("Total time:%d\n",(int)ev_now(loop));
    // 
    ev_timer_stop(loop, &timeout_watcherY);
    ev_timer_start(loop, &timeout_watcherX);
    ev_break(EV_A_ EVBREAK_ONE);
}

static void stdin_cb(EV_P_ ev_io *w, int revents){
    extern char *name;
    ev_timer_stop(loop, &timeout_watcherX);
    ev_timer_stop(loop, &timeout_watcherY);
    
    if(player){
        ev_timer_stop(loop, &timeout_watcherX);
        ev_timer_start(loop, &timeout_watcherY);
    }else{
        ev_timer_stop(loop, &timeout_watcherY);
        ev_timer_start(loop, &timeout_watcherX); 
    }
}
