#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.h"
#include "list.h"


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
    printf("Message Reading by name Done\n");
}
message *searchMessage(const char *username) {
    node *temp = head;
    while (temp != NULL) {
        register int i = 0;
        if(temp->type == MESSAGE) {
            message *message = temp->data;
            if(strcmp(message->username,username) == 0 && message->delivered == false) {
                checkMessage(message->toUsername);
                return message;
            }
        }
        temp = temp->next;
    }
    return NULL;
};

