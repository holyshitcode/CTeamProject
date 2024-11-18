#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

#include "list.h"


Board *initBoard(const char *writer, const char *contents) {
    Board *board = malloc(sizeof(Board));
    if (board == NULL) {
        return NULL;
    }
    strcpy(board->writer, writer);
    strcpy(board->contents, contents);
    board->boardHit = 0;
    board->likeCount = 0;
    board->comments = NULL;
    printf("Board Inited.");
    return board;
}

void insertBoard(Board *board) {
    insertData(board,BOARD);
    printf("Board Inserted.");
}

void showBoard() {
    showData(BOARD);
}
void saveBoard(Board *board,FILE *fp) {
    fprintf(fp,"%s %s %d %d %s\n",board->writer,board->contents,board->boardHit,board->likeCount,board->comments);
    printf("Board Saved.");
}
void loadBoard(FILE *fp) {
    Board *board = malloc(sizeof(Board));
    if (board != NULL) {
        fscanf(fp, "%s %s %d %d %s\n",board->writer,board->contents,&board->boardHit,&board->likeCount,board->comments);
        printf("Board Loaded.");
    }
    free(board);
}
void loadAllBoards(FILE *fp) {
    Board *board = malloc(sizeof(Board));
    if (board != NULL) {
        while(fp != NULL && !feof(fp)) {
            loadBoard(fp);
        }
        free(board);
    }
}



