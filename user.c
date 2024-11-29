#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "user.h"
#define GROUP_MAX 5

groups groupList;

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


int userRegister(FILE *fp,char* nickname, char* id, char* pwd) {
    node *temp = head;
    while (temp != NULL) {
        if(temp->type == STRUCT) {
            UserInfo* userInfo = temp->data;
            if (strcmp(userInfo->nickname, nickname) == 0) {
                return 0;
            }
        }
        temp = temp->next;
    }
    UserInfo *user = malloc(sizeof(struct UserInfo));
    if (user == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(user->nickname, nickname);
    strcpy(user->id, id);
    strcpy(user->passwd, pwd);

    fprintf(fp, "%s %s %s\n", nickname, id, pwd);

    insertData(user, STRUCT);
    printf("Register Process Done.\n");
    return 1;
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
    if (username == NULL || strlen(username) == 0) {
        printf("Error: Username cannot be empty.\n");
        return NULL;
    }

    UserInfo *leader = findUser(username);
    if (leader == NULL) {
        printf("Error: User '%s' not found. Only registered users can create groups.\n", username);
        return NULL;
    }

    group *newGroup = malloc(sizeof(group));
    if (newGroup == NULL) {
        fprintf(stderr, "Memory allocation failed for group.\n");
        return NULL;
    }

    newGroup->groupName = strdup(groupName);
    newGroup->leaderName = strdup(username);
    newGroup->userCount = 0;
    newGroup->users[newGroup->userCount++] = leader;

    for (int i = 0; i < 100; i++) {
        if (groupList.group[i] == NULL) {
            groupList.group[i] = newGroup;
            printf("Group '%s' created successfully with leader '%s'.\n", groupName, username);
            return newGroup;
        }
    }

    printf("Error: Group list is full. Cannot create group '%s'.\n", groupName);
    free(newGroup->groupName);
    free(newGroup->leaderName);
    free(newGroup);
    return NULL;
}


group *findGroup(const char *groupName) {
    for (int i = 0; i < 100; i++) {
        if (groupList.group[i] != NULL && strcmp(groupList.group[i]->groupName, groupName) == 0) {
            return groupList.group[i];
        }
    }
    return NULL;
}
void joinGroup(const char *groupName, const char *username) {
    group *group = findGroup(groupName);
    if (group == NULL) {
        printf("Group not found: %s\n", groupName);
        return;
    }

    if (group->userCount >= GROUP_MAX) {
        printf("Group is full: %s\n", groupName);
        return;
    }

    UserInfo *user = findUser(username);
    if (user == NULL) {
        printf("User not found: %s\n", username);
        return;
    }

    group->users[group->userCount++] = user;
    printf("User '%s' joined the group '%s'.\n", username, group->groupName);
}
void saveGroup(FILE *fp, const group *group) {
    fprintf(fp, "%s\n", group->groupName);
    fprintf(fp, "%s\n", group->leaderName);
    fprintf(fp, "%d\n", group->userCount);
    for (int i = 0; i < group->userCount; i++) {
        fprintf(fp, "%s %s %s\n",
                group->users[i]->nickname,
                group->users[i]->id,
                group->users[i]->passwd);
    }
    fprintf(fp, "END_GROUP\n");
}
void loadGroup(FILE *fp) {
    char groupName[50], leaderName[50];
    int userCount;

    // 파일 끝까지 반복
    while (fscanf(fp, "%s\n%s\n%d\n", groupName, leaderName, &userCount) == 3) {
        group *newGroup = malloc(sizeof(group));
        if (!newGroup) {
            fprintf(stderr, "Memory allocation failed for new group.\n");
            return;
        }

        // 그룹 이름과 리더 이름 동적 할당
        newGroup->groupName = strdup(groupName);
        newGroup->leaderName = strdup(leaderName);
        newGroup->userCount = 0;

        // 사용자 정보를 읽고 추가
        for (int i = 0; i < userCount; i++) {
            UserInfo *user = malloc(sizeof(UserInfo));
            if (!user) {
                fprintf(stderr, "Memory allocation failed for user.\n");
                free(newGroup);
                return;
            }

            if (fscanf(fp, "%s %s %s\n", user->nickname, user->id, user->passwd) != 3) {
                fprintf(stderr, "Failed to read user info correctly.\n");
                free(user);
                free(newGroup);
                return;
            }

            newGroup->users[newGroup->userCount++] = user;
        }

        // 그룹 리스트에 추가
        int stored = 0;
        for (int i = 0; i < 100; i++) {
            if (groupList.group[i] == NULL) {
                groupList.group[i] = newGroup;
                stored = 1;
                break;
            }
        }

        if (!stored) {
            fprintf(stderr, "Group list is full. Cannot add more groups.\n");
            free(newGroup->groupName);
            free(newGroup->leaderName);
            for (int i = 0; i < newGroup->userCount; i++) {
                free(newGroup->users[i]);
            }
            free(newGroup);
        }

        // "END_GROUP" 처리
        char endGroup[10];
        fscanf(fp, "%s", endGroup); // "END_GROUP" 읽기
        if (strcmp(endGroup, "END_GROUP") != 0) {
            fprintf(stderr, "Expected 'END_GROUP' but found '%s'.\n", endGroup);
            return;
        }
    }
}


int resetPwd(const char *fileName, const char *name, const char *id, const char *newPwd) {

    UserInfo *user = findUser(name);
    if (user != NULL && strcmp(user->id, id) == 0) {

        strcpy(user->passwd, newPwd);
        printf("'%s'님의 비밀번호가 성공적으로 업데이트 되었습니다.\n", name);

        FILE *fp = fopen(fileName, "w");

        node *temp = head;
        while (temp != NULL) {
            if (temp->type == STRUCT) {
                UserInfo *currentUser = temp->data;
                fprintf(fp, "%s %s %s\n", currentUser->nickname, currentUser->id, currentUser->passwd);
            }
            temp = temp->next;
        }

        fclose(fp);
        return 1;
    }

    printf("'%s'님의 id가 '%s'인 사용자를 찾지 못했습니다.\n", name, id);
    return 0;
}
