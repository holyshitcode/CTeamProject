#ifndef BOARD_H
#define BOARD_H

typedef struct Board {
    char writer[20];
    char *contents;
    int boardHit;
    int likeCount;
    char *comments;
}Board;


Board *initBoard(const char *writer, const char *contents);
void insertBoard(Board *board);
void displayBoard(Board *board);
void deleteBoard(Board *board);
void displayLikeCount(Board *board);
void displayComments(Board *board);

#endif // LIST_