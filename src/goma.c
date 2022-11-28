#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backprop.h"
#include "goma.h"
#include "board.h"

void generate_goma(){
    Pawn = (GOMA*)malloc(sizeof(GOMA));
    Pawn->syb = PAWN;
    // Pawn->act= &detect_Pawn;
    Lance = (GOMA*)malloc(sizeof(GOMA));
    Lance->syb = LANCE;
    // Lance->act = &detect_Lance;
    Knight = (GOMA*)malloc(sizeof(GOMA));
    Knight->syb = KNIGHT;
    // Knight->act = &detect_Knight;
    Sliver = (GOMA*)malloc(sizeof(GOMA));
    Sliver->syb = SLIVER;
    // Sliver->act = &detect_Sliver;
    Gold = (GOMA*)malloc(sizeof(GOMA));
    Gold->syb = GOLD;
    // Gold->act = &detect_Gold;
    King = (GOMA*)malloc(sizeof(GOMA));
    King->syb = KING;
    // King->act = &detect_King;
    Rook = (GOMA*)malloc(sizeof(GOMA));
    Rook->syb = ROOK;
    // Rook->act = &detect_Rook;
    Bishop = (GOMA*)malloc(sizeof(GOMA));
    Bishop->syb = BISHOP;
    // Bishop->act = &detect_Bishop;
    // Empty = (GOMA*)malloc(sizeof(GOMA));
}

BAG* init_bag(BAG* ptbag){
    ptbag = (BAG*)malloc(sizeof(BAG));
    ptbag->top = DEFAULT;
    return ptbag;
}

