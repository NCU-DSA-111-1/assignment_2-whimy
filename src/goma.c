#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "backprop.h"
#include "goma.h"
#include "board.h"

void generate_goma(){
    Pawn = (GOMA*)malloc(sizeof(GOMA));
    Pawn->syb = PAWN;
    Pawn->act = &detect_Pawn;
    Pawn->icon = 'p';
    Lance = (GOMA*)malloc(sizeof(GOMA));
    Lance->syb = LANCE;
    Lance->act = &detect_Lance;
    Lance->icon = 'l';
    Knight = (GOMA*)malloc(sizeof(GOMA));
    Knight->syb = KNIGHT;
    Knight->act = &detect_Knight;
    Knight->icon = 'n';
    Sliver = (GOMA*)malloc(sizeof(GOMA));
    Sliver->syb = SLIVER;
    Sliver->act = &detect_Sliver;
    Sliver->icon = 's';
    Gold = (GOMA*)malloc(sizeof(GOMA));
    Gold->syb = GOLD;
    Gold->act = &detect_Gold;
    Gold->icon = 'g';
    King = (GOMA*)malloc(sizeof(GOMA));
    King->syb = KING;
    King->act = &detect_King;
    King->icon = 'k';
    Rook = (GOMA*)malloc(sizeof(GOMA));
    Rook->syb = ROOK;
    Rook->act = &detect_Rook;
    Rook->icon = 'r';
    Bishop = (GOMA*)malloc(sizeof(GOMA));
    Bishop->syb = BISHOP;
    Bishop->act = &detect_Bishop;
    Bishop->icon = 'b';
    Empty = (GOMA*)malloc(sizeof(GOMA));
    Empty->syb = EMPTY;
}

BAG* init_bag(BAG* ptbag){
    ptbag = (BAG*)malloc(sizeof(BAG));
    ptbag->top = DEFAULT;
    return ptbag;
}

Direction detect_direction(char user, int dx,int dy){
    // pure direction
    if(dy == 0){
        if(dx > 0){
            return LEFT;
        }else{
            return RIGHT;
        }
    }
    // player dependent direction
    if(dy < 0){
        if(dx == 0){
            return (user == 'x' ? FORWARD : BACKWARD);
        }else if(dx > 0){
            return (user == 'x' ? FL : BL);
        }else{
            return (user == 'x' ? FR : BR);
        }
    }else if(dy > 0){
        if(dx == 0){
            return user == 'y' ? FORWARD : BACKWARD;
        }else if(dx > 0){
            return user == 'y' ? FL : BL;
        }else{
            return user == 'y' ? FR : BR;
        }
    }
    printf("Exit detect function without detecting.\n");
    return 0;
}


int detect_Pawn(char user,int* px,int* py, char detect_mode){
    // in pure detected mode
    int dx = *(px+1) - *px;
    int dy = *(py+1) - *py;
    Direction dir = detect_direction(user,dx,dy);
    if(dir == FORWARD && abs(dy) == 1){
        return 0;
    }
    else if(dir == BACKWARD){
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m步 cannot move backward.\nPlease try again.\n\n"): printf("\n");
        return 1;
    }else if(dir == FORWARD && abs(dy) > 1){
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m步 can only move forward one step\nPlease try again.\n\n"): printf("\n");
        return 1;
    }else{
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m步 cannot move horizontally\nPlease try again.\n\n"): printf("\n");
        return 1;
    }
    return 0;
}

int detect_Lance(char user,int* px,int* py, char detect_mode){
    // in pure detected mode
    int dx = *(px+1) - *px;
    int dy = *(py+1) - *py;
    Direction dir = detect_direction(user,dx,dy);
    if(dir == FORWARD){
        if(detect_path(px,py)){
            return 1;
        }else{
            return 0;
        }
    }else{
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m香 can only move foward.\nPlease try again.\n\n"): printf("\n");
        return 1;
    }
}

int detect_Knight(char user,int* px,int* py, char detect_mode){
    int dx = *(px+1) - *px;
    int dy = *(py+1) - *py;
    if(user == 'x' && ((abs(dx) != 1) || (dy != -2))){
        printf("\033[31mYou can not move to here.\n[Error 3]:\033[m桂 can only jumps at the foward angle of a 2x3 triangle.\nPlease try again.\n\n");
        return 1;
    }else if(user == 'y' && ((abs(dx) != 1) || (dy != 2))){
        printf("\033[31mYou can not move to here.\n[Error 3]:\033[m桂 can only jumps at the foward angle of a 2x3 triangle.\nPlease try again.\n\n");
        return 1;
    }else{
        return 0;
    }
}

int detect_Sliver(char user,int* px,int* py, char detect_mode){
    int dx = *(px+1) - *px;
    int dy = *(py+1) - *py;
    Direction dir = detect_direction(user,dx,dy);
    if(dir == LEFT || dir == RIGHT){
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m銀 cannot move just right or left.\nPlease try again.\n\n"): printf("\n");
        return 1;
    }else if(abs(dy) > 1 || abs(dx) > 1){
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m銀 can only move within one square around itself.\nPlease try again.\n\n"): printf("\n");
        return 1;
    }else if((user == 'x' && dy == 1) || (user == 'y' && -dy == 1)){
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m銀 cannot move right backward.\nPlease try again.\n\n"): printf("\n");
        return 1;
    }else{
        return 0;
    }
}

int detect_Gold(char user,int* px,int* py, char detect_mode){
    int dx = *(px+1) - *px;
    int dy = *(py+1) - *py;
    if(abs(dy) > 1 || abs(dx) > 1){
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m金 can only move within one square around itself.\nPlease try again.\n\n"): printf("\n");
        return 1;
    }else if((user == 'x' && (dy == 1 && abs(dx) == 1)) || (user == 'y' && (-dy == 1 && abs(dx) == 1))){
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m金 cannot move diagonally backwards.\nPlease try again.\n\n"): printf("\n");
        return 1;
    }else{
        return 0;
    }
}

int detect_King(char user,int* px,int* py, char detect_mode){
    int dx = *(px+1) - *px;
    int dy = *(py+1) - *py;
    if(abs(dx) > 1 || abs(dy) > 1){
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m玉 can only move within one square.\nPlease try again.\n\n"): printf("\n");
        return 1;
    }else{
        return 0;
    }
}

int detect_Rook(char user,int* px,int* py, char detect_mode){
    int dx = *(px+1) - *px;
    int dy = *(py+1) - *py;
    if( dx*dy != 0){
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m飛 can only move straight horizontally or vertically.\nPlease try again.\n\n"): printf("\n");
        return 1;       
    }else if(detect_path(px,py)){
        return 1;
    }else{
        return 0;
    }
}

int detect_Bishop(char user,int* px,int* py, char detect_mode){
    int dx = *(px+1) - *px;
    int dy = *(py+1) - *py;
    if(abs(dx) != abs(dy)){
        (detect_mode == OFF) ? printf("\n\033[31mYou can not move to here.\n[Error 3]:\033[m角 can only move in a diagonal direction.\nPlease try again.\n\n"): printf("\n");
        return 1;
    }else if(detect_path(px,py)){
        return 1;
    }else{
        return 0;
    }
}

int detect_path(int* px,int *py){
    int dx = *(px+1) - *px;
    int dy = *(py+1) - *py;
    // pure direction
    if(dy == 0){    // pure x motivation
        if(dx < 0){
            for(int t = 1;t < abs(dx);t++){
                if(ptboard[*py - 1][ROW - *px - t].goma != NULL){
                    printf("\033[31mYou can not move to here.\n[Error 4]:\033[mThere is chess in your moving path.\nPlease try again.\n\n");
                    return 1;
                }
            }
        }else{
            for(int t = 1;t < abs(dx);t++){
                if(ptboard[*py - 1][ROW - *px + t].goma != NULL){
                    printf("\033[31mYou can not move to here.\n[Error 5]:\033[mThere is chess in your moving path.\nPlease try again.\n\n");
                    return 1;
                }
            }
        }
    }else if(dx == 0){  // pure y motivation
        if(dy > 0){
            for(int t = 1;t < abs(dy);t++){
                if(ptboard[*py + t - 1][ROW - *px].goma != NULL){
                    printf("\033[31mYou can not move to here.\n[Error 6]:\033[mThere is chess in your moving path.\nPlease try again.\n\n");
                    return 1;
                }
            }
        }else{
            for(int t = 1;t < abs(dy);t++){
                if(ptboard[*py - t - 1][ROW - *px].goma != NULL){
                    printf("\033[31mYou can not move to here.\n[Error 7]:\033[mThere is chess in your moving path.\nPlease try again.\n\n");
                    return 1;
                }
            }            
        }
    }else if(dy > 0){   // Down
        if(dx > 0){     // Left
            for(int t = 1;t < abs(dy);t++){
                if(ptboard[*py + t - 1][ROW - *px + t].goma != NULL){
                    printf("\033[31mYou can not move to here.\n[Error 8]:\033[mThere is chess in your moving path.\nPlease try again.\n\n");
                    return 1;
                }
            }
        }else{  // dx < 0
            for(int t = 1;t < abs(dy);t++){
                if(ptboard[*py + t - 1][ROW - *px - t].goma != NULL){
                    printf("\033[31mYou can not move to here.\n[Error 9]:\033[mThere is chess in your moving path.\nPlease try again.\n\n");
                    return 1;
                }
            }
        }
    }else{   // UP
        if(dx > 0){     // Left
            for(int t = 1;t < abs(dy);t++){
                if(ptboard[*py - t - 1][ROW - *px + t].goma != NULL){
                    printf("\033[31mYou can not move to here.\n[Error 10]:\033[mThere is chess in your moving path.\nPlease try again.\n\n");
                    return 1;
                }
            }
        }else{  // dx < 0
            for(int t = 1;t < abs(dy);t++){
                if(ptboard[*py - t - 1][ROW - *px - t].goma != NULL){
                    printf("\033[31mYou can not move to here.\n[Error 11]:\033[mThere is chess in your moving path.\nPlease try again.\n\n");
                    return 1;
                }
            }
        }
    }
    return 0;
}
