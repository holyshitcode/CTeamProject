#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "list.h"
#include "message.h"
#define GROUP_MAX 5



typedef struct Group {
    char *leaderName;
    char *groupName;
    UserInfo *users[GROUP_MAX];
    int userCount;
}group;

typedef struct GroupList {
    group group[100];
}groups;

groups groupList;

UserInfo *findUser(const char *name);

int resetAndUpdatePwd(FILE *fp, FILE *tempFp, const char *name, const char *id, const char *newPwd) {
    if (!fp || !tempFp) {
        fprintf(stderr, "Invalid file pointers provided.\n");
        return 0;
    }

    char fileNickname[20];
    char fileId[30];
    char filePasswd[30];
    int isUpdated = 0;

    // 파일 읽기 및 수정
    while (fscanf(fp, "%s %s %s", fileNickname, fileId, filePasswd) == 3) {
        if (strcmp(fileNickname, name) == 0 && strcmp(fileId, id) == 0) {
            fprintf(tempFp, "%s %s %s\n", fileNickname, fileId, newPwd); // 새 비밀번호 기록
            isUpdated = 1;

            // 메모리 업데이트
            UserInfo *user = findUser(name);
            if (user != NULL && strcmp(user->id, id) == 0) {
                strcpy(user->passwd, newPwd);
            }
        } else {
            fprintf(tempFp, "%s %s %s\n", fileNickname, fileId, filePasswd);
        }
    }

    if (isUpdated) {
        printf("'%s'님의 비밀번호가 성공적으로 업데이트 되었습니다.\n", name);
        return 1;
    } else {
        printf("'%s'님의 id가 '%s'인 사용자를 찾지 못했습니다.\n", name, id);
        return 0;
    }
}


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

UserInfo *findUser(const char *name) {
    node *temp = head;
    while (temp != NULL) {
        if (temp->type == STRUCT) {
            UserInfo *user = temp->data;
            if (strcmp(user->nickname, name) == 0) {
                return user;
            }
        }
        temp = temp->next;
    }
    return NULL;
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

group *createGroup(const char *groupName, const char *username) {
    // 메모리 할당 및 초기화
    group *newGroup = malloc(sizeof(group));
    if (newGroup == NULL) {
        fprintf(stderr, "Memory allocation failed for group.\n");
        exit(EXIT_FAILURE);
    }
    newGroup->groupName = strdup(groupName);
    newGroup->leaderName = strdup(username);
    newGroup->userCount = 0;

    // 리더를 그룹의 첫 번째 사용자로 추가
    UserInfo *leader = findUser(username);
    if (leader == NULL) {
        fprintf(stderr, "Leader not found: %s\n", username);
        free(newGroup->groupName);
        free(newGroup->leaderName);
        free(newGroup);
        return NULL;
    }
    newGroup->users[newGroup->userCount++] = leader;

    printf("Group '%s' created with leader '%s'.\n", groupName, username);
    return newGroup;
}
group *findGroup(const char *groupName) {
    for (int i = 0; i < 100; i++) {
        if (groupList.group[i].groupName != NULL &&
            strcmp(groupList.group[i].groupName, groupName) == 0) {
            return &groupList.group[i];
            }
    }
    return NULL;
}
void joinGroup(const char *groupName, const char *username) {
    group *group = findGroup(groupName); // groupName으로 그룹 찾기
    if (group == NULL) {
        printf("Group not found: %s\n", groupName);
        return;
    }

    if (group->userCount >= GROUP_MAX) {
        printf("Group is full: %s\n", groupName);
        return;
    }

    UserInfo *user = findUser(username); // 사용자 검색
    if (user == NULL) {
        printf("User not found: %s\n", username);
        return;
    }

    group->users[group->userCount++] = user; // 사용자 추가
    printf("User '%s' joined the group '%s'.\n", username, group->groupName);
}
void saveGroup(FILE *fp, const group *group) {
    fprintf(fp, "%s\n", group->groupName);    // 그룹 이름
    fprintf(fp, "%s\n", group->leaderName);   // 리더 이름
    fprintf(fp, "%d\n", group->userCount);    // 사용자 수
    for (register int i = 0; i < group->userCount; i++) {
        fprintf(fp, "%s %s %s\n",
                group->users[i]->nickname,
                group->users[i]->id,
                group->users[i]->passwd);     // 사용자 정보 저장
    }
    fprintf(fp, "END_GROUP\n");               // 그룹 끝
}
void loadGroup(FILE *fp) {
    char groupName[50], leaderName[50];
    int userCount;

    if (fscanf(fp, "%s\n%s\n%d\n", groupName, leaderName, &userCount) != 3) {
        printf("Error reading group data.\n");
        return;
    }

    group *newGroup = malloc(sizeof(group));
    if (newGroup == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newGroup->groupName = strdup(groupName);
    newGroup->leaderName = strdup(leaderName);
    newGroup->userCount = 0;

    for (int i = 0; i < userCount; i++) {
        UserInfo *user = malloc(sizeof(UserInfo));
        if (user == NULL) {
            fprintf(stderr, "Memory allocation failed for user.\n");
            break;
        }
        if (fscanf(fp, "%s %s %s\n", user->nickname, user->id, user->passwd) != 3) {
            printf("Error reading user data for group: %s.\n", groupName);
            free(user);
            break;
        }
        newGroup->users[newGroup->userCount++] = user;
    }

    // 그룹을 groupList에 저장
    for (int i = 0; i < 100; i++) {
        if (groupList.group[i].groupName == NULL) {
            groupList.group[i] = *newGroup;  // 데이터 복사
            printf("Loaded group: %s (Leader: %s, Members: %d)\n",
                   newGroup->groupName, newGroup->leaderName, newGroup->userCount);
            free(newGroup);  // 구조체 자체는 복사되었으므로 메모리 해제
            return;
        }
    }

    printf("Group list is full. Cannot add more groups.\n");
    free(newGroup->groupName);
    free(newGroup->leaderName);
    free(newGroup);
}

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
    // FILE *fp2 = fopen("info.txt", "r"); // 파일을 읽기 모드로 엽니다.
    //
    // loadAllMembers(fp2);
    // char *result = findPwd("gg", "gg");
    // fclose(fp2);
    // printf("%s\n",result);



    // Board *board = initBoard("gg", "gg");
    // insertBoard(board);
    // showData(BOARD);
    // showData(STRUCT);
    // FILE *fp3 = fopen("group.txt", "r");
    // message *newMessage = initMessage("junyeong","a","hello");
    // insertMessage(newMessage);
    // printMessage("junyeong");
    //
    // UserInfo user1 = {"AliceTest", "alice123test", "password1t"};
    // UserInfo user2 = {"Bob22Test", "bob456test", "password2t"};
    // insertData(&user1, STRUCT);
    // insertData(&user2, STRUCT);
    //
    // group *newGroup = createGroup("StudyGroup", "Alice");
    // if (newGroup != NULL) {
    //     printf("Group '%s' created successfully!\n", newGroup->groupName);
    // }
    // saveGroup(fp3, newGroup);

    // free(newGroup->groupName);
    // free(newGroup->leaderName);
    // free(newGroup);
    // loadGroup(fp3);
    //
    // joinGroup("StudyGroup","AliceTest");
    // joinGroup("StudyGroup","Bob22Test");
    // fclose(fp3);
    // FILE *fp4 = fopen("group.txt", "a");
    // group *testGroup = findGroup("StudyGroup");
    // saveGroup(fp4, testGroup);
    // free(testGroup);
    // fclose(fp4);

    FILE *fp = fopen("info.txt", "r");
    FILE *tempFp = fopen("temp.txt", "w");

    if (!fp || !tempFp) {
        fprintf(stderr, "Error opening files.\n");
        if (fp) fclose(fp);
        if (tempFp) fclose(tempFp);
        return 1;
    }

    char name[50], id[50], newPwd[50];
    printf("이름을 입력해 주세요: ");
    scanf("%49s", name);
    printf("ID를 입력해 주세요: ");
    scanf("%49s", id);
    printf("새로운 비밀번호를 입력해 주세요: ");
    scanf("%49s", newPwd);

    if (resetAndUpdatePwd(fp, tempFp, name, id, newPwd)) {
        printf("Password reset successfully.\n");

        // 파일 교체
        fclose(fp);
        fclose(tempFp);
        remove("info.txt");
        rename("temp.txt", "info.txt");
    } else {
        printf("Password reset failed.\n");
        fclose(fp);
        fclose(tempFp);
        remove("temp.txt"); // 실패 시 임시 파일 제거
    }

    // 메모리 상 사용자 출력
    printf("Updated user data:\n");
    showData(STRUCT);

    return 0;
}