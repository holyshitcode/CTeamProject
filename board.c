#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

#include "list.h"

/**
 *  "board.c"
 *  Produced by 유지헌
 *  Managed by 김준영
 */


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
    if (board == NULL) {
        fprintf(stderr, "Memory allocation failed for board\n");
        return;
    }

    // 메모리 할당
    board->contents = malloc(256); // 가정: 최대 256자
    board->comments = malloc(256); // 가정: 최대 256자
    if (board->contents == NULL || board->comments == NULL) {
        fprintf(stderr, "Memory allocation failed for board contents/comments\n");
        free(board->contents);
        free(board->comments);
        free(board);
        return;
    }

    // 데이터 읽기
    if (fscanf(fp, "%19s %255s %d %d %255s",
               board->writer,
               board->contents,
               &board->boardHit,
               &board->likeCount,
               board->comments) == 5) {
        insertBoard(board);
               } else {
                   free(board->contents);
                   free(board->comments);
                   free(board);
               }
}

void loadAllBoards(FILE *fp) {
    if (fp == NULL) {
        fprintf(stderr, "File pointer is NULL\n");
        return;
    }

    while (!feof(fp)) {
        loadBoard(fp);
    }
}



