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
    printf("Board Inited.");
    return board;
}

void insertBoard(Board *board) {
    insertData(board,BOARD);
    printf("Board Inserted.");
}

void showBoard(Board *board) {
    showData(BOARD);
}



