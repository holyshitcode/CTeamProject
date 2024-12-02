#ifndef LIST_H
#define LIST_H

/**
 *  "list.h"
 *  Produced by 김준영
 *  Managed by 김준영
 */

extern char loginUserName[21];

typedef enum {
    STRING,
    INTEGER,
    CHAR,
    STRUCT,
    BOARD,
    MESSAGE,
    ALL
} dataType;


typedef struct node {
    void *data;
    struct node *next;
    dataType type;
} node;

typedef struct UserInfo {
    char nickname[20];
    char id[30];
    char passwd[30];
}UserInfo;




extern node *head;
extern node *tail;


void insertData(void *data, dataType type);
void showData(dataType type);
void deleteData(void *data, dataType type);
void freeList();
node *searchData(void *data, dataType type);
int userLogin(const char *id, const char *pwd);
#endif