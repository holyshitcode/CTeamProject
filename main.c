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

    UserInfo *user = malloc(sizeof(struct UserInfo));
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

void loadMember(FILE *fp) {
    static int counter = 0;
    UserInfo *user = malloc(sizeof(struct UserInfo));
    char nickname[20];
    char id[30];
    char passwd[30];

    if (fscanf(fp, "%s %s %s\n", nickname, id, passwd) != 3) {
        return;  
    }

    strcpy(user->nickname, nickname);
    strcpy(user->id, id);
    strcpy(user->passwd, passwd);
    insertData(user, STRUCT);
    printf("Load Process Done.(%d)\n",++counter);
}

void loadAllMembers(FILE *fp) {
    while (fp != NULL && !feof(fp)) {
        loadMember(fp);
    }
    printf("Loading Done.\n");
    showData(STRUCT);
}

int main(void) {
    char tempUserId[30], tempUserPwd[30]; // 메모리를 배열로 할당
    FILE *fp = fopen("info.txt", "a"); // 파일을 읽기 모드로 엽니다.
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    userRegister(fp);
    fclose(fp);
    printf("Please enter your id: ");
    scanf("%s", tempUserId);
    printf("Please enter your password: ");
    scanf("%s", tempUserPwd);

    if(userLogin(tempUserId, tempUserPwd)) {
        printf("Login success");
    }
    FILE *fp2 = fopen("info.txt", "r"); // 파일을 읽기 모드로 엽니다.

    loadAllMembers(fp2);

    fclose(fp2);
    return 0;
}