// Board information, update board, goma_move
#ifndef __BOARD_H__
#define __BOARD_H__

#include "goma.h"

#define ROW 9
#define COLUMN 9
#define NA 'z'

typedef struct board_t{
    char owner;
    GOMA* goma;
}BOARD;

void init_board(BOARD** ptb);
void visualize_board(BOARD** const ptb);
void swap(BOARD* origin, BOARD *goal);

#endif