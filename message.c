#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.h"
#include "list.h"


message *initMessage(char *username, char *toUsername, char *contents) {
    message *message = malloc(sizeof(message));
    strcpy(message->username,username);
    strcpy(message->toUsername,toUsername);
    strcpy(message->contents,contents);
    message->delivered = false;
    printf("Message has been added\n");
    return message;
}
void checkMessage(const char *toUsername) {
    node *temp = head;
    while (temp != NULL) {
        if(temp->type == MESSAGE) {
            message *message = temp->data;
            if(strcmp(message->toUsername,toUsername) == 0) {
                printf("Message opened!");
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
    node *temp = head;
    while (temp != NULL) {
        if(temp->type == MESSAGE) {
            deleteData(message,MESSAGE);
        }
    }
};
void printMessage(message message);
message *searchMessage(char *username);

