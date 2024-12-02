#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boardGui.h"
#include "user.h"
#include "list.h"

#define MAX_LEN 30

/**
 *  "loginGui.c"
 *  Produced by 김 준, 김준영
 *  Managed by 김준영
 */

void drawLoginButton(WINDOW *win, int focus);
void drawRegisterButton(WINDOW *win, int focus);
void drawLoginWindow(WINDOW *win, int focus, const char *msg, const char *id, const char *passwd);
void drawRegisterWindow(WINDOW *win, int focus, const char *msg, const char *id, const char *passwd, const char *nickname);
void handleLogin(WINDOW *win);
void handleRegister(WINDOW *win);

void drawButtonsLogin(WINDOW *win, int focus) {
    int buttonStartY = 9;
    int buttonStartX = 17;

    if (focus == 2) {
        wattron(win, A_REVERSE);
    }
    mvwprintw(win, buttonStartY, buttonStartX, "Login");
    if (focus == 2) {
        wattroff(win, A_REVERSE);
    }

    buttonStartY = 11;
    buttonStartX = 13;
    if (focus == 3) {
        wattron(win, A_REVERSE);
    }
    mvwprintw(win, buttonStartY, buttonStartX, "Go To Register");
    if (focus == 3) {
        wattroff(win, A_REVERSE);
    }

    wrefresh(win);
}

void drawLoginWindow(WINDOW *win, int focus, const char *msg, const char *id, const char *passwd) {
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(win, 1, 12, "Welcome to Blog!");

    if (focus == 0) {
        wattron(win, A_REVERSE);
        mvwprintw(win, 3, 3, "ID:");
        mvwprintw(win, 3, 6, "%s", id);
        wattroff(win, A_REVERSE);
    } else {
        mvwprintw(win, 3, 3, "ID:");
        mvwprintw(win, 3, 6, "%s", id);
    }

    if (focus == 1) {
        wattron(win, A_REVERSE);
        mvwprintw(win, 5, 3, "Password:");
        mvwprintw(win, 5, 12, "%s", passwd);
        wattroff(win, A_REVERSE);
    } else {
        mvwprintw(win, 5, 3, "Password:");
        mvwprintw(win, 5, 12, "%s", passwd);
    }

    mvwprintw(win, 7, 1, "                                     ");
    mvwprintw(win, 7, 3, msg);

    drawButtonsLogin(win, focus);

    wrefresh(win);
}

void drawButtonsRegister(WINDOW *win, int focus) {
    int buttonStartY = 11;
    int buttonStartX = 19;

    if (focus == 3) {
        wattron(win, A_REVERSE);
    }

    mvwprintw(win, buttonStartY, buttonStartX, "Register");

    if (focus == 3) {
        wattroff(win, A_REVERSE);
    }

    buttonStartY = 13;
    buttonStartX = 18;

    if (focus == 4) {
        wattron(win, A_REVERSE);
    }

    mvwprintw(win, buttonStartY, buttonStartX, "Go To Login");

    if (focus == 4) {
        wattroff(win, A_REVERSE);
    }

    wrefresh(win);
}

void drawRegisterWindow(WINDOW *win, int focus, const char *msg, const char *id, const char *passwd, const char *nickname) {
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(win, 1, 12, "Register a New Account");

    if (focus == 0) {
        wattron(win, A_REVERSE);
        mvwprintw(win, 3, 3, "Nickname:");
        mvwprintw(win, 3, 12, "%s", nickname);
        wattroff(win, A_REVERSE);
    } else {
        mvwprintw(win, 3, 3, "Nickname:");
        mvwprintw(win, 3, 12, "%s", nickname);
    }

    if (focus == 1) {
        wattron(win, A_REVERSE);
        mvwprintw(win, 5, 3, "ID:");
        mvwprintw(win, 5, 6, "%s", id);
        wattroff(win, A_REVERSE);
    } else {
        mvwprintw(win, 5, 3, "ID:");
        mvwprintw(win, 5, 6, "%s", id);
    }

    if (focus == 2) {
        wattron(win, A_REVERSE);
        mvwprintw(win, 7, 3, "Password:");
        mvwprintw(win, 7, 12, "%s", passwd);
        wattroff(win, A_REVERSE);
    } else {
        mvwprintw(win, 7, 3, "Password:");
        mvwprintw(win, 7, 12, "%s", passwd);
    }

    mvwprintw(win, 9, 1, "                                     ");
    mvwprintw(win, 9, 3, msg);

    drawButtonsRegister(win, focus);

    wrefresh(win);
}

void handleLogin(WINDOW *win) {
    int focus = 0;
    char id[MAX_LEN] = "";
    char passwd[MAX_LEN] = "";
    char message[100] = "Please enter your ID and Password.";

    int loginHeight = 14, loginWidth = 40;
    int loginStartY = 5, loginStartX = 40;
    WINDOW *loginWin = newwin(loginHeight, loginWidth, loginStartY, loginStartX);

    if (loginWin == NULL) {
        fprintf(stderr, "Error: Unable to create login window.\n");
        return;
    }

    drawLoginWindow(loginWin, focus, message, id, passwd);

    while (1) {
        int ch = wgetch(loginWin);

        if (ch == 'w' || ch == 'W') {
            focus = (focus - 1 + 4) % 4;
        } else if (ch == 's' || ch == 'S') {
            focus = (focus + 1) % 4;
        } else if (ch == 10) {
            if (focus == 0) {
                mvwprintw(loginWin, 3, 6, "                          ");
                wrefresh(loginWin);
                mvwgetnstr(loginWin, 3, 6, id, sizeof(id) - 1);
                id[sizeof(id) - 1] = '\0';
            } else if (focus == 1) {
                mvwprintw(loginWin, 5, 12, "                          ");
                wrefresh(loginWin);
                mvwgetnstr(loginWin, 5, 12, passwd, sizeof(passwd) - 1);
                passwd[sizeof(passwd) - 1] = '\0';
            } else if (focus == 2) {
                if (userLogin(id, passwd)) {
                    strcpy(message, "Login successful!");
                    clear();
                    refresh();
                    handleDashboard(win);
                    return;
                } else {
                    strcpy(message, "Login failed!");
                }
            } else if (focus == 3) {
                clear();
                refresh();
                handleRegister(win);
                return;
            }
        }

        drawLoginWindow(loginWin, focus, message, id, passwd);
    }
}

void handleRegister(WINDOW *win) {
    int registerFocus = 0;
    char nickname[MAX_LEN] = "";
    char id[MAX_LEN] = "";
    char passwd[MAX_LEN] = "";
    char registerMessage[100] = "Enter your new Nickname, ID, and Password.";

    int registerHeight = 16, registerWidth = 47;
    int registerStartY = 5, registerStartX = 40;
    WINDOW *registerWin = newwin(registerHeight, registerWidth, registerStartY, registerStartX);

    drawRegisterWindow(registerWin, registerFocus, registerMessage, id, passwd, nickname);

    while (1) {
        int ch = wgetch(registerWin);

        if (ch == 'w' || ch == 'W') {
            registerFocus = (registerFocus == 0) ? 4 : registerFocus - 1;
        } else if (ch == 's' || ch == 'S') {
            registerFocus = (registerFocus == 4) ? 0 : registerFocus + 1;
        } else if (ch == 10) {
            if (registerFocus == 0) {
                mvwprintw(registerWin, 3, 12, "                ");
                wrefresh(registerWin);
                echo();
                mvwgetnstr(registerWin, 3, 12, nickname, sizeof(nickname) - 1);
                noecho();
            } else if (registerFocus == 1) {
                mvwprintw(registerWin, 5, 6, "                ");
                wrefresh(registerWin);
                echo();
                mvwgetnstr(registerWin, 5, 6, id, sizeof(id) - 1);
                noecho();
            } else if (registerFocus == 2) {
                mvwprintw(registerWin, 7, 12, "                ");
                wrefresh(registerWin);
                echo();
                mvwgetnstr(registerWin, 7, 12, passwd, sizeof(passwd) - 1);
                noecho();
            } else if (registerFocus == 3) {
                FILE *fp = fopen("userInfo.txt","a+");

                if (1) {
                    userRegister(fp, nickname, id, passwd);
                    fclose(fp);
                    strcpy(registerMessage, "Registration successful!");
                    clear();
                    refresh();
                    handleLogin(win);
                    return;
                } else {
                    strcpy(registerMessage, "ID already exists.");
                }
            } else if (registerFocus == 4) {
                clear();
                refresh();
                handleLogin(win);
                return;
            }
        }

        drawRegisterWindow(registerWin, registerFocus, registerMessage, id, passwd, nickname);
    }
}