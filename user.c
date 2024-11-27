#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "list.h"
#include "message.h"
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
        if (groupList.group[i]->groupName != NULL &&
            strcmp(groupList.group[i]->groupName, groupName) == 0) {
            return groupList.group[i];
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

    while (fscanf(fp, "%s\n%s\n%d\n", groupName, leaderName, &userCount) == 3) {
        group *newGroup = malloc(sizeof(group));
        if (!newGroup) {
            fprintf(stderr, "Memory allocation failed.\n");
            return;
        }

        newGroup->groupName = strdup(groupName);
        newGroup->leaderName = strdup(leaderName);
        newGroup->userCount = 0;

        for (int i = 0; i < userCount; i++) {
            UserInfo *user = malloc(sizeof(UserInfo));
            if (!user || fscanf(fp, "%s %s %s\n", user->nickname, user->id, user->passwd) != 3) {
                free(user);
                break;
            }
            newGroup->users[newGroup->userCount++] = user;
        }

        // 그룹 저장
        int stored = 0;
        for (int i = 0; i < 100; i++) {
            if (!groupList.group[i]) {
                groupList.group[i] = newGroup;
                stored = 1;
                break;
            }
        }

        if (!stored) {
            fprintf(stderr, "Group list is full. Cannot add more groups.\n");
            free(newGroup->groupName);
            free(newGroup->leaderName);
            free(newGroup);
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
        return 1; // 성공
    }

    printf("'%s'님의 id가 '%s'인 사용자를 찾지 못했습니다.\n", name, id);
    return 0; // 실패
}

void showGroupMembers(const char *groupName) {
    for (int i = 0; i < GROUP_MAX; i++) {
        // 그룹 이름이 유효하고, 입력된 그룹 이름과 일치하는지 확인
        if (strlen(groupList.group[i]->groupName) > 0 && strcmp(groupList.group[i]->groupName, groupName) == 0) {
            printf("Group: %s (Leader: %s)\n", groupList.group[i]->groupName, groupList.group[i]->leaderName);
            printf("Members:\n");
            for (int j = 0; j < groupList.group[i]->userCount; j++) {
                if (groupList.group[i]->users[j] != NULL) {
                    printf("- %s\n", groupList.group[i]->users[j]->nickname); // 멤버 이름 출력
                }
            }
            return;
        }
    }
    printf("Group '%s' not found.\n", groupName);
}