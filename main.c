#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "list.h"
#include "message.h"
#include "user.h"




int main(void) {
    char tempUserId[30], tempUserPwd[30]; // 메모리를 배열로 할당
    // FILE *fp = fopen("info.txt", "a"); // 파일을 읽기 모드로 엽니다.
    // if (fp == NULL) {
    //     printf("Error opening file.\n");
    //     return 0;
    // }
    //
    // userRegister(fp);
    // fclose(fp);
    // printf("Please enter your id: ");
    // scanf("%s", tempUserId);
    // printf("Please enter your password: ");
    // scanf("%s", tempUserPwd);
    //
    // if(userLogin(tempUserId, tempUserPwd)) {
    //     printf("Login success");
    // }
    FILE *fp2 = fopen("info.txt", "r"); // 파일을 읽기 모드로 엽니다.

    loadAllMembers(fp2);
    char *result = findPwd("gg", "gg");
    fclose(fp2);
    printf("%s\n",result);



    // Board *board = initBoard("gg", "gg");
    // insertBoard(board);
    // showData(BOARD);
    // showData(STRUCT);
    FILE *fp3 = fopen("group.txt", "r");
    message *newMessage = initMessage("junyeong","a","hello");
    insertMessage(newMessage);
    printMessage("junyeong");

    UserInfo user1 = {"AliceTest", "alice123test", "password1t"};
    UserInfo user2 = {"Bob22Test", "bob456test", "password2t"};
    insertData(&user1, STRUCT);
    insertData(&user2, STRUCT);
    //
    // group *newGroup = createGroup("StudyGroup", "Alice");
    // if (newGroup != NULL) {
    //     printf("Group '%s' created successfully!\n", newGroup->groupName);
    // }
    // saveGroup(fp3, newGroup);

    // free(newGroup->groupName);
    // free(newGroup->leaderName);
    // free(newGroup);
    loadGroup(fp3);

    joinGroup("StudyGroup","AliceTest");
    joinGroup("StudyGroup","Bob22Test");
    fclose(fp3);
    FILE *fp4 = fopen("group.txt", "a");
    group *testGroup = findGroup("StudyGroup");
    saveGroup(fp4, testGroup);
    free(testGroup);
    fclose(fp4);



    return 0;
}