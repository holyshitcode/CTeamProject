#ifndef MESSAGE_H
#define MESSAGE_H
#include <stdbool.h>
typedef struct Message {
    char *username;
    char *toUsername;
    char *contents;
    bool delivered;
}message;
message *initMessage(char *username, char *toUsername, char *contents);
void insertMessage(message message);
void deleteMessage(message message);
void printMessage(message message);
message *searchMessage(char *username);
void checkMessage(const char *toUsername);

#endif