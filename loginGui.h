
#ifndef LOGINGUI_H
#define LOGINGUI_H
#include <curses.h>


void drawLoginButton(WINDOW *win, int focus);
void drawRegisterButton(WINDOW *win, int focus);
void drawLoginWindow(WINDOW *win, int focus, const char *msg, const char *id, const char *passwd);
void drawRegisterWindow(WINDOW *win, int focus, const char *msg, const char *id, const char *passwd);
void handleLogin(WINDOW *win);
void handleRegister(WINDOW *win);
void drawButtonsRegister(WINDOW *win, int focus);
void drawButtonsLogin(WINDOW *win, int focus);

#endif