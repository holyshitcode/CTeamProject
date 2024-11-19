#ifndef MESSAGE_H
#define MESSAGE_H
#include <stdbool.h>
typedef struct Message {
    char *username;
    char *toUsername;
    char *contents;
    bool delivered;
}message;
message *initMessage(const char *username, const char *toUsername, const char *contents);
void insertMessage(message *message);
void deleteMessage(message *message);
void printMessage(const char *username);
message *searchMessage(const char *username);
void checkMessage(const char *toUsername);

#endif