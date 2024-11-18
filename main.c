#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"



void userRegister(FILE *fp) {
    char nickname[20];
    char id[30];
    char passwd[30];

    printf("Please enter your nickname: ");
    scanf("%s", nickname);
    printf("Please enter your id: ");
    scanf("%s", id);
    printf("Please enter your passwd: ");
    scanf("%s", passwd);

    struct UserInfo *user = malloc(sizeof(struct UserInfo));
    if (user == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(user->nickname, nickname);
    strcpy(user->id, id);
    strcpy(user->passwd, passwd);
    fprintf(fp, "%s %s %s\n", nickname, id, passwd);

    insertData(user, STRUCT);
    printf("Register Process Done.\n");
}



int main(void) {
    char tempUserId[30], tempUserPwd[30]; // 메모리를 배열로 할당
    FILE *fp = fopen("info.txt", "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    userRegister(fp);

    printf("Please enter your id: ");
    scanf("%s", tempUserId);
    printf("Please enter your password: ");
    scanf("%s", tempUserPwd);

    if(userLogin(tempUserId, tempUserPwd)) {
        printf("Login success");
    }

    fclose(fp);
    return 0;
}