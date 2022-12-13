#ifndef __BACKPROP_H__
#define __BACKPROP_H__

#include "board.h"

#define print_y printf()

#define TRUE 1
#define FALSE 0
#define DEFAULT -1
#define X 0
#define Y 1
#define ON 'y'
#define OFF 'n'



void game_start(char *gname);
void goma_move(int user,char *gname);
void print_step(BOARD* sel, int x, int y);
void free_mem();

#endif
