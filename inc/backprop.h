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
void goma_move(char *gname);
void print_step(BOARD* sel, int x, int y);
void free_mem();
static void timeout_cbX(EV_P_ ev_timer *w, int revents);
static void timeout_cbY(EV_P_ ev_timer *w, int revents);
static void stdin_cb(EV_P_ ev_io *w, int revents);

#endif
