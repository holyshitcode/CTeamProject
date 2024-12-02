#ifndef MESSAGE_H
#define MESSAGE_H
#include <stdbool.h>

/**
 *  "message.h"
 *  Produced by 김준영
 *  Managed by 김준영
 */

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
void saveMessagesToFile(FILE *fp);
void loadMessagesFromFile(FILE *fp);
#endif