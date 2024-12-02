#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "list.h"
#include "message.h"
#define GROUP_MAX 5

/**
 *  "user.h"
 *  Produced by 김준영
 *  Managed by 김준영
 */

typedef struct Group {
    char *leaderName;
    char *groupName;
    UserInfo *users[GROUP_MAX];
    int userCount;
}group;

typedef struct GroupList {
    group *group[100];
}groups;

groups groupList;


char* findPwd(char* name, char* id);
int userRegister(FILE *fp,char* nickname, char* id, char* pwd);
UserInfo *findUser(const char *name);
void loadMember(FILE *fp);
void loadAllMembers(FILE *fp) ;
group *createGroup(const char *groupName, const char *username) ;
group *findGroup(const char *groupName) ;
void joinGroup(const char *groupName, const char *username) ;
void saveGroup(FILE *fp, const group *group) ;
void loadGroup(FILE *fp) ;
int resetPwd(const char *fileName, const char *name, const char *id, const char *newPwd);
void showGroupMembers(const char *groupName);
#endif