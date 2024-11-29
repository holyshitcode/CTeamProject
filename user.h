#ifndef USER_H
#define USER_H

#include <stdio.h>
#include "list.h"

#define GROUP_MAX 5



typedef struct Group {
    char *leaderName;
    char *groupName;
    UserInfo *users[GROUP_MAX];
    int userCount;
}group;

typedef struct GroupList {
    group *group[100];
}groups;

extern groups groupList;


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
#endif