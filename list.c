#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#include "board.h"
#include "message.h"


// 전역 변수 정의
node *head = NULL;
node *tail = NULL;
char *loginUserName = NULL;

void insertData(void *data, dataType type) {
    node *temp = malloc(sizeof(node));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    temp->data = data;
    temp->type = type;
    temp->next = NULL;

    if (head == NULL) {
        head = temp;
        tail = temp;
    } else {
        tail->next = temp;
        tail = temp;
    }
}

void showData(dataType type) {
    node *temp = head; // 리스트의 시작점
    while (temp != NULL) {
        if (temp->data != NULL && temp->type == type) { // 데이터 타입 비교
            switch (type) {
                case STRING:
                    printf("String Data: %s\n", (char *)temp->data);
                break;
                case INTEGER:
                    printf("Integer Data: %d\n", *(int *)temp->data);
                break;
                case CHAR:
                    printf("Character Data: %c\n", *(char *)temp->data);
                break;
                case STRUCT: {
                    UserInfo *userData = (UserInfo *)temp->data;
                    printf("User Info: %s\n", userData->nickname);
                    break;
                }
                case BOARD: {
                    Board *boardData = (Board *)temp->data;
                    printf("Writer = %s\n", boardData->writer);
                    printf("Contents = %s\n", boardData->contents ? boardData->contents : "(null)");
                    printf("Likes = %d\n", boardData->likeCount);
                    printf("BoardHits = %d\n", boardData->boardHit);
                    printf("Comments = %s\n", boardData->comments ? boardData->comments : "(null)");
                    break;
                }
                default:
                    printf("Unknown Data Type.\n");
                break;
            }
        }
        temp = temp->next; // 다음 노드로 이동
    }
}

void deleteData(void *data, dataType type) {
    node *temp = head;
    node *prev = NULL;

    while (temp != NULL) {
        if (temp->type == type) {
            bool toDelete = false;
            switch (type) {
                case STRING:
                    if (strcmp(temp->data, data) == 0)
                        toDelete = true;
                    break;
                case INTEGER:
                    if (*(int *)temp->data == *(int *)data)
                        toDelete = true;
                    break;
                case CHAR:
                    if (*(char *)temp->data == *(char *)data)
                        toDelete = true;
                    break;
                case STRUCT: {
                    UserInfo *userData = temp->data;
                    UserInfo *compareData = data;
                    if (strcmp(compareData->nickname, userData->nickname) == 0 && strcmp(compareData->passwd,userData->passwd)==0) {
                        toDelete = true;
                    }
                    break;
                }
                case BOARD: {
                    Board *boardData = temp->data;
                    Board *compareData = data;
                    if(strcmp(compareData->writer, boardData->writer)==0) {
                        toDelete = true;
                    }
                    break;
                }
                case MESSAGE: {
                    message *messageData = temp->data;
                    message *compareData = data;
                    if(strcmp(messageData->username, compareData->username)==0 && strcmp(messageData->toUsername, compareData->toUsername)==0
                        && compareData->delivered == true) {
                        toDelete = true;
                    }
                }
                default: break;
            }

            if (toDelete) {
                if (prev == NULL) {
                    head = temp->next;
                    if (temp == tail) {
                        tail = NULL;
                    }
                } else {
                    prev->next = temp->next;
                    if (temp == tail) {
                        tail = prev;
                    }
                }

                free(temp->data);
                free(temp);
                printf("Data deleted successfully.\n");
                return;
            }
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Data not found.\n");
}

void freeList() {
    node *temp = head;
    while (temp != NULL) {
        node *next = temp->next;
        free(temp->data);
        free(temp);
        temp = next;
    }
    head = tail = NULL;
}
int userLogin(const char *id, const char *pwd) {
    const node *temp = head;
    while (temp != NULL) {
        if(temp->type == STRUCT) {
            const UserInfo *userTemp = temp->data;
            if (strcmp(id, userTemp->id) == 0) {
                if(strcmp(pwd,userTemp->passwd) == 0){
                    strcpy(loginUserName,userTemp->nickname);
                    return 1;
                }
            }
        }
        temp = temp->next;
    }
    return 0;
}
node *searchData(void *data, dataType type) {
    node *temp = head;

    while (temp != NULL) {
        if (temp->type == type) {
            switch (type) {
                case STRING:
                    if (strcmp(temp->data, data) == 0) {
                        return temp;
                    }
                break;
                case INTEGER:
                    if (*(int *)temp->data == *(int *)data) {
                        return temp;
                    }
                break;
                case CHAR:
                    if (*(char *)temp->data == *(char *)data) {
                        return temp;
                    }

                break;
                default: break;
            }
        }
        temp = temp->next;
    }

    return NULL; // 데이터가 리스트에 없으면 NULL 반환
}


