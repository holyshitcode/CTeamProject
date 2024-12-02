#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boardGui.h"
#include "user.h"
#include "list.h"
#include "message.h"
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

    // Login Button
    if (focus == 2) {
        wattron(win, A_REVERSE); // 하이라이트
    }
    mvwprintw(win, buttonStartY, buttonStartX, "Login");
    if (focus == 2) {
        wattroff(win, A_REVERSE); // 하이라이트 해제
    }

    // Register Button
    buttonStartY = 11;
    buttonStartX = 13;
    if (focus == 3) {
        wattron(win, A_REVERSE); // 하이라이트
    }
    mvwprintw(win, buttonStartY, buttonStartX, "Go To Register");
    if (focus == 3) {
        wattroff(win, A_REVERSE); // 하이라이트 해제
    }

    wrefresh(win);
}


// 로그인 창을 그리는 함수
void drawLoginWindow(WINDOW *win, int focus, const char *msg, const char *id, const char *passwd) {
    box(win, 0, 0);
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

    // Register Button
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


// 회원가입 창을 그리는 함수
void drawRegisterWindow(WINDOW *win, int focus, const char *msg, const char *id, const char *passwd, const char *nickname) {
    box(win, 0, 0);
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

// 로그인 및 회원가입 메인 화면을 관리하는 함수
void handleLogin(WINDOW *win) {
    int focus = 0; // 0: ID, 1: Password, 2: Login Button, 3: Register Button
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
        int ch = wgetch(loginWin); // 키 입력 받기

        if (ch == 'w' || ch == 'W') {
            focus = (focus - 1 + 4) % 4; // 포커스 위로 이동
        } else if (ch == 's' || ch == 'S') {
            focus = (focus + 1) % 4; // 포커스 아래로 이동
        } else if (ch == 10) { // Enter 키
            if (focus == 0) {
                mvwgetnstr(loginWin, 3, 6, id, sizeof(id) - 1);
                id[sizeof(id) - 1] = '\0';
            } else if (focus == 1) {
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
    int registerFocus = 0; // 0: Nickname, 1: ID, 2: Password, 3: Register Button, 4: Back to Login Button
    char nickname[MAX_LEN] = "";
    char id[MAX_LEN] = "";
    char passwd[MAX_LEN] = "";
    char registerMessage[100] = "Enter your new Nickname, ID, and Password.";

    // 회원가입 창 크기 설정
    int registerHeight = 16, registerWidth = 47;
    int registerStartY = 5, registerStartX = 40;
    WINDOW *registerWin = newwin(registerHeight, registerWidth, registerStartY, registerStartX);

    // 회원가입 창을 그립니다.
    drawRegisterWindow(registerWin, registerFocus, registerMessage, id, passwd, nickname);

    // 포커스 이동과 입력을 처리하는 루프
    while (1) {
        int ch = wgetch(registerWin); // 키 입력 받기

        // 포커스 이동 처리
        if (ch == 'w' || ch == 'W') {
            registerFocus = (registerFocus == 0) ? 4 : registerFocus - 1;
        } else if (ch == 's' || ch == 'S') {
            registerFocus = (registerFocus == 4) ? 0 : registerFocus + 1;
        } else if (ch == 10) { // Enter 키 처리
            if (registerFocus == 0) {
                // Nickname 입력
                mvwprintw(registerWin, 3, 12, "                "); // 기존 입력 삭제
                wrefresh(registerWin);
                echo();
                mvwgetnstr(registerWin, 3, 12, nickname, sizeof(nickname) - 1);
                noecho();
            } else if (registerFocus == 1) {
                // ID 입력
                mvwprintw(registerWin, 5, 6, "                "); // 기존 입력 삭제
                wrefresh(registerWin);
                echo();
                mvwgetnstr(registerWin, 5, 6, id, sizeof(id) - 1);
                noecho();
            } else if (registerFocus == 2) {
                // Password 입력
                mvwprintw(registerWin, 7, 12, "                "); // 기존 입력 삭제
                wrefresh(registerWin);
                echo();
                mvwgetnstr(registerWin, 7, 12, passwd, sizeof(passwd) - 1);
                noecho();
            } else if (registerFocus == 3) {
                FILE *fp = fopen("userInfo.txt","a+");
                // Register 버튼 처리
                if (1) {
                    userRegister(fp,nickname,id, passwd);
                    fclose(fp);
                    strcpy(registerMessage, "Registration successful!");
                    clear();
                    refresh();
                    handleLogin(win); // 로그인 화면으로 돌아가기
                    return;
                } else {
                    strcpy(registerMessage, "ID already exists.");
                }
            } else if (registerFocus == 4) {
                // "Back to Login" 버튼 처리
                clear();
                refresh();
                handleLogin(win);
                return;
            }
        }

        // 회원가입 창을 다시 그립니다.
        drawRegisterWindow(registerWin, registerFocus, registerMessage, id, passwd, nickname);
    }
}