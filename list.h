#ifndef LIST_H
#define LIST_H
#include <stdbool.h>

/**
 *  "list.h"
 *  Produced by 김준영
 *  Managed by 김준영
 */

char *loginUserName[21];
/**
 * 노드의 데이터타입을 구분하는 enum
 */
typedef enum {
    STRING,
    INTEGER,
    CHAR,
    STRUCT,
    BOARD,
    MESSAGE,
    ALL
} dataType;

/**
 * 노드 구조체 정의
 */
typedef struct node {
    void *data; // generic으로 데이터를 저장
    struct node *next;
    dataType type;
} node;

typedef struct UserInfo {
    char nickname[20];
    char id[30];
    char passwd[30];
}UserInfo;



// 전역 변수 선언
extern node *head;
extern node *tail;

/**
 * 함수 선언
 */
void insertData(void *data, dataType type);
void showData(dataType type);
void deleteData(void *data, dataType type);
void freeList();
node *searchData(void *data, dataType type);
int userLogin(const char *id, const char *pwd);
#endif // LIST_H