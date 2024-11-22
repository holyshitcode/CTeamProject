#ifndef USER_H
#define USER_H

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


char* findPwd(char* name, char* id);
void userRegister(FILE *fp);
UserInfo *findUser(const char *name);
void loadMember(FILE *fp);
void loadAllMembers(FILE *fp) ;
group *createGroup(const char *groupName, const char *username) ;
group *findGroup(const char *groupName) ;
void joinGroup(const char *groupName, const char *username) ;
void saveGroup(FILE *fp, const group *group) ;
void loadGroup(FILE *fp) ;


#endif