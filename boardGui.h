#ifndef BOARDGUI_H
#define BOARDGUI_H

#include <curses.h>


void handleDashboard(WINDOW *win);
void loadBoardData(FILE *fp);
void addNewPost(WINDOW *win, int width, FILE *fp);
void viewPost(WINDOW *win, int width);
void showBoardList(WINDOW *win, int width);
void printCentered(WINDOW *win, int y, int width, const char *text);
void displayUserInfo(WINDOW *win, const char *username, const char *groupName);
char *isGroup(char *username);
WINDOW* createCenteredWindow(int height, int width);
#endif