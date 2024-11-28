#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boardGui.h"
#include "loginGui.h"
#include "list.h"
#include "user.h"
#include "board.h"
#include "message.h"


#define MAX_LEN 30
#define MAX_POSTS 5
#define MAX_MESSAGES 3




typedef struct {
    char sender[MAX_LEN];
    char message[256];
} Message;

Message messages[MAX_MESSAGES] = { {"", ""}, {"", ""}, {"", ""} };


void printCentered(WINDOW *win, int y, int width, const char *text) {
    int len = strlen(text);
    int x = (width - len) / 2;
    mvwprintw(win, y, x, "%s", text);
}

void showBoardList(WINDOW *win, int width) {
    int y = 2;
    node *temp = head;
    int index = 1;

    while (temp != NULL) {
        if (temp->type == BOARD) {
            Board *board = (Board *) temp->data;
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "%d. %s %10s", index++, board->writer, board->contents);
            mvwprintw(win, y++, 2, "%s", buffer);
        }
        temp = temp->next;

    }

    wrefresh(win);
}


void viewPost(WINDOW *win, int width) {
    mvwprintw(win, 2, 2, "Enter post number: ");
    char numStr[10];
    wgetstr(win, numStr);
    int postNum = atoi(numStr) - 1;

    if (postNum >= 0) {
        node *temp = head;
        int index = 0;
        while (temp != NULL) {
            if (temp->type == BOARD) {
                if (index == postNum) {
                    Board *board = (Board *) temp->data;
                    wclear(win);
                    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
                    mvwprintw(win, 2, 2, "Post by: %s", board->writer);
                    mvwprintw(win, 3, 2, "Content: %s", board->contents);
                    mvwprintw(win, 5, 2, "Likes: %d | Hits: %d", board->likeCount, board->boardHit);
                    wrefresh(win);
                    wgetch(win);
                    return;
                }
                index++;
            }
            temp = temp->next;
        }
    }

    mvwprintw(win, 7, 2, "Invalid post number.");
    wrefresh(win);
    wgetch(win);
}

void addNewPost(WINDOW *win, int width, FILE *fp) {
    char writer[20], content[256];
    echo();
    mvwprintw(win, 2, 2, "Writer: ");
    wgetstr(win, writer);
    mvwprintw(win, 3, 2, "Content: ");
    wgetstr(win, content);
    noecho();


    Board *newPost = initBoard(writer, content);
    if (newPost == NULL) {
        mvwprintw(win, 5, 2, "Failed to create new post.");
        wrefresh(win);
        wgetch(win);
        return;
    }


    insertBoard(newPost);


    if (fp != NULL) {
        saveBoard(newPost, fp);
        fflush(fp);
        mvwprintw(win, 5, 2, "Post added and saved successfully!");
    } else {
        mvwprintw(win, 5, 2, "Post added, but file saving failed.");
    }

    wrefresh(win);
    wgetch(win);
}


void loadBoardData(FILE *fp) {
    if (fp == NULL) {
        return;
    }

    loadAllBoards(fp);
}

void handleGroupMenu(WINDOW *win);
void displayUserInfo(WINDOW *win, const char *username, const char *groupName) {
    mvwprintw(win, 1, 8, "BLOG");
    mvwprintw(win, 3, 2, "User: %s", username);
    mvwprintw(win, 4, 2, "Group: %s", groupName);
    wrefresh(win);
}
char *isGroup(char *username) {
    if (username == NULL) {
        return NULL;
    }

    for (int i = 0; i < 100; i++) {
        if (groupList.group[i]==NULL) {
            continue;
        }

        for (int j = 0; j < 5; j++) {
            if (groupList.group[i]->users[j] == NULL) {
                continue;
            }

            if (strcmp(groupList.group[i]->users[j]->nickname, username) == 0) {
                return groupList.group[i]->groupName;
            }
        }
    }

    return NULL;
}
void sendMessage(WINDOW *win) {
    char toUsername[MAX_LEN], contents[256];
    echo();

    mvwprintw(win, 10, 2, "To Username: ");
    wgetstr(win, toUsername);

    mvwprintw(win, 11, 2, "Message: ");
    wgetstr(win, contents);
    noecho();

    message *newMsg = initMessage(loginUserName, toUsername, contents);
    if (newMsg != NULL) {
        insertMessage(newMsg);
        FILE *messageFp =fopen("message.txt","a");
        saveMessagesToFile(messageFp);
        fclose(messageFp);
        mvwprintw(win, 13, 2, "Message sent successfully!");
    } else {
        mvwprintw(win, 13, 2, "Failed to send message.");
    }

    wrefresh(win);
    wgetch(win);
}
void viewMessages(WINDOW *win) {
    wclear(win);
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');

    int y = 2;
    int messageCount = 0;

    FILE *messageFp = fopen("message.txt", "r");
    if (messageFp != NULL) {
        loadMessagesFromFile(messageFp);
        fclose(messageFp);
    }

    while (true) {
        message *msg = searchMessage(loginUserName);
        if (msg == NULL) {
            break;
        }

        mvwprintw(win, y++, 2, "From: %s", msg->username);
        mvwprintw(win, y++, 2, "Message: %s", msg->contents);
        mvwprintw(win, y++, 2, "----");
        messageCount++;
    }

    if (messageCount == 0) {
        mvwprintw(win, 2, 2, "No messages to display.");
    } else {
        FILE *messageSave = fopen("message.txt", "w");
        if (messageSave != NULL) {
            saveMessagesToFile(messageSave);
            fclose(messageSave);
        }
        mvwprintw(win, y, 2, "All messages displayed. Press any key to go back.");
    }

    wrefresh(win);
    wgetch(win);
}

void handleMessageMenu(WINDOW *win) {
    char *options[] = {
        "Send Message",
        "View Messages",
        "Back"
    };

    int selected = 0;
    int ch;

    while (1) {

        wclear(win);
        wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
        mvwprintw(win, 1, 2, "Message Menu:");

        for (int i = 0; i < 3; i++) {
            if (i == selected) {
                wattron(win, A_REVERSE);
                mvwprintw(win, 3 + i, 2, "%s", options[i]);
                wattroff(win, A_REVERSE);
            } else {
                mvwprintw(win, 3 + i, 2, "%s", options[i]);
            }
        }

        wrefresh(win);

        ch = wgetch(win);


        if (ch == 'w' || ch == 'W') {
            selected = (selected == 0) ? 2 : selected - 1;
        } else if (ch == 's' || ch == 'S') {
            selected = (selected == 2) ? 0 : selected + 1;
        } else if (ch == 10) {
            if (selected == 0) {

                sendMessage(win);
            } else if (selected == 1) {

                viewMessages(win);
            } else if (selected == 2) {

                return;
            }
        }
    }
}

void handleDashboard(WINDOW *win) {
    clear();
    refresh();

    int dashboardHeight = 25, dashboardWidth = 50;
    int dashboardStartY = 2, dashboardStartX = 10;

    int leftWidth = 20, rightWidth = 80;
    int totalWidth = leftWidth + rightWidth;
    int totalHeight = dashboardHeight;

    WINDOW *leftWin = newwin(totalHeight, leftWidth, dashboardStartY, dashboardStartX);
    WINDOW *rightWin = newwin(totalHeight, rightWidth, dashboardStartY, dashboardStartX + leftWidth);

    wborder(leftWin, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(rightWin, '|', '|', '-', '-', '+', '+', '+', '+');

    char *username= loginUserName;

    char *groupname = isGroup(username);
    if (groupname == NULL) {
        groupname = "empty";
    }
    displayUserInfo(leftWin, username, groupname);

    char *menu[] = {
        "Group",
        "Board",
        "Message",
        "Logout",
        "Exit"
    };

    for (int i = 0; i < 5; i++) {
        printCentered(leftWin, 12 + i, leftWidth, menu[i]);
    }

    wattron(leftWin, A_REVERSE);
    printCentered(leftWin, 12 + 0, leftWidth, menu[0]);
    wattroff(leftWin, A_REVERSE);

    wrefresh(leftWin);
    wrefresh(rightWin);

    FILE *fp = fopen("board.txt", "a+");
    if (fp == NULL) {
        printCentered(rightWin, 2, rightWidth, "Failed to open board file.");
        wrefresh(rightWin);
        return;
    }
    loadBoardData(fp);

    int selected = 0;
    int focusBox = 0;
    int ch;

    while ((ch = wgetch(leftWin)) != 'q') {
        if (focusBox == 0) {
            if (ch == 'w' || ch == 'W') {
                selected = (selected == 0) ? 4 : selected - 1;
            } else if (ch == 's' || ch == 'S') {
                selected = (selected == 4) ? 0 : selected + 1;
            } else if (ch == 10) {
                focusBox = 1;
                wclear(rightWin);
                wborder(rightWin, '|', '|', '-', '-', '+', '+', '+', '+');
                if (selected == 0) {
                    handleGroupMenu(rightWin);
                }
                if (selected == 1) {
                    printCentered(rightWin, 1, rightWidth, "Board Posts:");
                    showBoardList(rightWin, rightWidth);

                    mvwprintw(rightWin, 20, 2, "Press 'n' to add new post, 'v' to view post, 'b' to go back.");
                    wrefresh(rightWin);

                    int boardCh;
                    while ((boardCh = wgetch(rightWin)) != 'b') {
                        if (boardCh == 'n') {
                            addNewPost(rightWin, rightWidth, fp);
                            wclear(rightWin);
                            wborder(rightWin, '|', '|', '-', '-', '+', '+', '+', '+');
                            printCentered(rightWin, 1, rightWidth, "Board Posts:");
                            showBoardList(rightWin, rightWidth);
                            mvwprintw(rightWin, 20, 2, "Press 'n' to add new post, 'v' to view post, 'b' to go back.");
                        } else if (boardCh == 'v') {
                            viewPost(rightWin, rightWidth);
                            wclear(rightWin);
                            wborder(rightWin, '|', '|', '-', '-', '+', '+', '+', '+');
                            printCentered(rightWin, 1, rightWidth, "Board Posts:");
                            showBoardList(rightWin, rightWidth);
                            mvwprintw(rightWin, 20, 2, "Press 'n' to add new post, 'v' to view post, 'b' to go back.");
                        }
                    }
                } else if (selected == 3) {
                    clear();
                    refresh();
                    fclose(fp);
                    handleLogin(win);
                    return;
                } else if (selected == 4) {
                    fclose(fp);
                    endwin();
                    return;
                }else if(selected == 2) {
                    handleMessageMenu(rightWin);
                }

                wrefresh(rightWin);
                focusBox = 0;
            }
        }

        for (int i = 0; i < 5; i++) {
            if (i == selected && focusBox == 0) {
                wattron(leftWin, A_REVERSE);
                printCentered(leftWin, 12 + i, leftWidth, menu[i]);
                wattroff(leftWin, A_REVERSE);
            } else {
                printCentered(leftWin, 12 + i, leftWidth, menu[i]);
            }
        }

        wrefresh(leftWin);
    }

    fclose(fp);
    endwin();
}
void createNewGroup(WINDOW *win) {
    char groupName[MAX_LEN], leaderName[MAX_LEN];
    echo();
    mvwprintw(win, 10, 2, "Enter Group Name: ");
    wgetstr(win, groupName);
    mvwprintw(win, 11, 2, "Enter Leader Name: ");
    wgetstr(win, leaderName);
    noecho();

    if (1) {
        group *group =createGroup(groupName, leaderName);
        FILE *groupFp = fopen("group.txt", "a+");
        saveGroup(groupFp,group);
        fclose(groupFp);
        mvwprintw(win, 13, 2, "Group '%s' created successfully.", groupName);
    } else {
        mvwprintw(win, 13, 2, "Failed to create group '%s'.", groupName);
    }

    wrefresh(win);
    wgetch(win);
}

void joinExistingGroup(WINDOW *win) {
    char groupName[MAX_LEN], userName[MAX_LEN];
    echo();
    mvwprintw(win, 10, 2, "Enter Group Name: ");
    wgetstr(win, groupName);
    mvwprintw(win, 11, 2, "Enter Your Username: ");
    wgetstr(win, userName);
    noecho();


    group *existingGroup = findGroup(groupName);
    if (existingGroup == NULL) {
        mvwprintw(win, 13
            , 2, "Group '%s' not found.", groupName);
        wrefresh(win);
        wgetch(win);
        return;
    }


    joinGroup(groupName, userName);


    FILE *groupFp2 = fopen("group.txt", "a");
    if (groupFp2 == NULL) {
        mvwprintw(win, 5, 2, "Failed to open group file for writing.");
        wrefresh(win);
        wgetch(win);
        return;
    }

    saveGroup(groupFp2, existingGroup);
    fclose(groupFp2);

    mvwprintw(win, 5, 2, "Successfully joined group '%s'.", groupName);
    wrefresh(win);
    wgetch(win);
}

void viewGroupMembers(WINDOW *win) {
    char groupName[MAX_LEN];
    echo();
    mvwprintw(win, 10, 2, "Enter Group Name: ");
    wgetstr(win, groupName);
    noecho();

    wclear(win);
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
    FILE *viewGroupFp = fopen("group.txt", "r");
    loadGroup(viewGroupFp);
    fclose(viewGroupFp);
    showGroupMembers(groupName);
    mvwprintw(win, 13, 2, "Members of group '%s' are displayed above.", groupName);
    wrefresh(win);
    wgetch(win);
}

void handleGroupMenu(WINDOW *win) {
    char *options[] = {
        "Create Group",
        "Join Group",
        "View Group Members",
        "Back"
    };

    int selected = 0;
    int ch;

    while (1) {

        wclear(win);
        wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
        mvwprintw(win, 1, 2, "Group Menu:");

        for (int i = 0; i < 4; i++) {
            if (i == selected) {
                wattron(win, A_REVERSE);
                mvwprintw(win, 3 + i, 2, "%s", options[i]);
                wattroff(win, A_REVERSE);
            } else {
                mvwprintw(win, 3 + i, 2, "%s", options[i]);
            }
        }

        wrefresh(win);

        ch = wgetch(win);


        if (ch == 'w' || ch == 'W') {
            selected = (selected == 0) ? 3 : selected - 1;
        } else if (ch == 's' || ch == 'S') {
            selected = (selected == 3) ? 0 : selected + 1;
        } else if (ch == 10) {
            if (selected == 0) {

                createNewGroup(win);
            } else if (selected == 1) {

                joinExistingGroup(win);
            } else if (selected == 2) {

                viewGroupMembers(win);
            } else if (selected == 3) {

                return;
            }
        }
    }
}