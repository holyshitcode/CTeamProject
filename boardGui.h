#ifndef BOARDGUI_H
#define BOARDGUI_H

#include <ncurses.h>

// handleDashboard 함수 선언
void handleDashboard(WINDOW *win);
void loadBoardData(FILE *fp);
void addNewPost(WINDOW *win, int width, FILE *fp);
void viewPost(WINDOW *win, int width);
void showBoardList(WINDOW *win, int width);
void printCentered(WINDOW *win, int y, int width, const char *text);
#endif