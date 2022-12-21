#ifndef __CHESS_H__
#define __CHESS_H__

#define print_xgoma(x) printf("\033[31m%s\033[m ",x)
#define print_ygoma(y) printf("\033[34m%s\033[m ",y)

#define GOMANUM 20
#define PAWN "步"
#define LANCE "香"
#define KNIGHT "桂"
#define SLIVER "銀"
#define GOLD "金"
#define KING "玉"
#define ROOK "飛"
#define BISHOP "角"
#define EMPTY "　"

typedef int (*func)(char,int*,int*,char);

typedef struct goma_t{
    char icon;
    char *syb;
    func act;
}GOMA;

typedef struct bag_t{
    GOMA mochi[GOMANUM];
    int top;
}BAG;

typedef enum Direction_t
{
    FORWARD,BACKWARD,LEFT,RIGHT,FL,FR,BL,BR
}Direction;

GOMA *Pawn, *Lance, *Knight, *Sliver, *Gold, *King, *Rook, *Bishop, *Empty;
BAG *x_bag, *y_bag;

void generate_goma();
BAG* init_bag(BAG* ptbag);
Direction detect_direction(char user, int dx,int dy);
int detect_Pawn(char user,int* px,int* py, char detect_mode);
int detect_Lance(char user,int* px,int* py, char detect_mode);
int detect_Knight(char user,int* px,int* py, char detect_mode);
int detect_Sliver(char user,int* px,int* py, char detect_mode);
int detect_Gold(char user,int* px,int* py, char detect_mode);
int detect_King(char user,int* px,int* py, char detect_mode);
int detect_Rook(char user,int* px,int* py, char detect_mode);
int detect_Bishop(char user,int* px,int* py, char detect_mode);
int detect_path(int* px,int *py);

#endif
