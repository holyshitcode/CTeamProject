#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "list.h"
#include "message.h"
#include "user.h"
#define GROUP_MAX 5



char* findPwd(char* name, char* id) {
    node *temp = head;
    while (temp != NULL) {
        if(temp->type == STRUCT) {
            UserInfo* userInfo = temp->data;
            if (strcmp(userInfo->id, id) == 0 && strcmp(userInfo->nickname, name) == 0) {
                return userInfo->passwd;
            }
        }
        temp = temp->next;
    }
    return NULL;
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
        if(temp->type == STRUCT) {
            UserInfo *user = temp->data;
            if(strcmp(user->nickname,name) == 0) {
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