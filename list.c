#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"



// 전역 변수 정의
node *head = NULL;
node *tail = NULL;

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

void showData() {
    node *temp = head;
    while (temp != NULL) {
        if (temp->data != NULL) {
            if (temp->type == STRING) {
                printf("String Data: %s\n", (char *)temp->data);
            } else if (temp->type == INTEGER) {
                printf("Integer Data: %d\n", *(int *)temp->data);
            } else if (temp->type == CHAR) {
                printf("Character Data: %c\n", *(char *)temp->data);
            }
        }
        temp = temp->next;
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
    node *temp = head;
    while (temp != NULL) {
        if(temp->type == STRUCT) {
            UserInfo *userTemp = (UserInfo *)temp->data;
            if (strcmp(id, userTemp->id) == 0) {
                if(strcmp(pwd,userTemp->passwd) == 0){
                    return 1;
                }
            }
        }
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