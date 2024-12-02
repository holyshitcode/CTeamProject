#ifndef BOARD_H
#define BOARD_H

/**
 *  "board.h"
 *  Produced by 유지헌
 *  Managed by 김준영
 */

typedef struct Board {
    char writer[20];
    char *contents;
    int boardHit;
    int likeCount;
    char *comments;
}Board;


Board *initBoard(const char *writer, const char *contents);
void insertBoard(Board *board);
void showBoard();
void saveBoard(Board *board,FILE *fp);
void loadBoard(FILE *fp);
void loadAllBoards(FILE *fp);


#endif