#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.h"
#include "list.h"

/**
 *  "message.c"
 *  Produced by 김준영
 *  Managed by 김준영
 */

message *initMessage(const char *username, const char *toUsername, const char *contents) {
    message *newMessage = malloc(sizeof(message));
    if (newMessage == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newMessage->username = strdup(username);
    newMessage->toUsername = strdup(toUsername);
    newMessage->contents = strdup(contents);
    newMessage->delivered = false;

    printf("Message has been added\n");
    return newMessage;
}
void checkMessage(const char *toUsername) {
    node *temp = head;
    while (temp != NULL) {
        if(temp->type == MESSAGE) {
            message *message = temp->data;
            if(strcmp(message->toUsername,toUsername) == 0) {
                printf("Message opened!\n");
                message->delivered = true;
            }
        }
        temp = temp->next;
    }
}
void insertMessage(message *message) {
    insertData(message,MESSAGE);
    printf("Message entered!\n");
}
void deleteMessage(message *message) {
    const node *temp = head;
    while (temp != NULL) {
        if(temp->type == MESSAGE) {
            deleteData(message,MESSAGE);
        }
    }
};
void printMessage(const char *username) {
    while(true) {
        message *foundMessage = searchMessage(username);
        if(foundMessage == NULL) {
            break;
        }
        printf("writer=%s\ncontents=%s\n",foundMessage->username,foundMessage->contents);

    }
    printf("Message Reading  Done\n");
}
message *searchMessage(const char *username) {
    node *temp = head;
    while (temp != NULL) {
        register int i = 0;
        if(temp->type == MESSAGE) {
            message *message = temp->data;
            if(strcmp(message->toUsername,username) == 0 && message->delivered == false) {
                checkMessage(message->toUsername);
                return message;
            }
        }
        temp = temp->next;
    }
    return NULL;
}

void saveMessagesToFile(FILE *fp) {// 파일을 쓰기 모드로 엽니다.
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file for writing");
        return;
    }

    node *temp = head;
    while (temp != NULL) {
        if (temp->type == MESSAGE) {
            message *msg = (message *)temp->data;
            fprintf(fp, "%s,%s,%s,%d\n",
                    msg->username,       // 발신자
                    msg->toUsername,    // 수신자
                    msg->contents,      // 메시지 내용
                    msg->delivered);    // 전달 상태 (0 또는 1)
        }
        temp = temp->next;
    }

    fclose(fp);
    printf("Messages saved to file");
}

void loadMessagesFromFile(FILE *fp) {
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file for reading");
        return;
    }

    char username[50];
    char toUsername[50];
    char contents[256];
    int delivered;

    while (fscanf(fp, "%49[^,],%49[^,],%255[^,],%d\n",
                  username, toUsername, contents, &delivered) == 4) {
        message *msg = initMessage(username, toUsername, contents);
        msg->delivered = delivered; // 전달 상태를 복원
        insertMessage(msg);         // 메시지를 리스트에 추가
                  }

    fclose(fp);
    printf("Messages loaded from file");
}

