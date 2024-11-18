#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

#include "list.h"


Board *initBoard(const char *writer, const char *content) {
    Board *board = malloc(sizeof(Board));
    if (board == NULL) {
        return NULL;
    }
    strcpy(board->writer, writer);

    board->contents = malloc(strlen(content) + 1); // 메모리 할당
    if (board->contents == NULL) {
        free(board);
        return NULL;
    }
    strcpy(board->contents, content);

    board->boardHit = 0;
    board->likeCount = 0;
    board->comments = NULL;
    printf("Board Inited.\n");
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



