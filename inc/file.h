#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>
#include <board.h>

// linked list
struct node{
    BOARD old_pos;
    BOARD new_pos;
    int cx[2];
    int cy[2];
    struct node *next;
};
typedef struct node NODE;


void Open_new_game(FILE* fp,char *fname);
void Reload_old_game(FILE* fp,char *fname);
NODE* Add_to_list(NODE* head, int *px, int *py);
NODE* Remove_from_list(NODE* head, BAG *ptbag);
void write_file(FILE* fp, char *fname, NODE *head, int* px, int* py);
void read_file(FILE* fp,char *fname);

#endif
