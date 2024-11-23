// login_register.h
#ifndef LOGIN_REGISTER_H
#define LOGIN_REGISTER_H

// 함수 선언
int userLogin(const char *id, const char *passwd);  // 로그인 함수
int userRegister(char *id, char *passwd);          // 회원가입 함수
void drawLoginButton(WINDOW *win, int focus);      // 로그인 버튼 그리기
void drawRegisterButton(WINDOW *win, int focus);   // 회원가입 버튼 그리기
void drawLoginWindow(WINDOW *win, int focus, const char *msg, const char *id, const char *passwd);  // 로그인 창 그리기
void drawRegisterWindow(WINDOW *win, int focus, const char *msg, const char *id, const char *passwd);  // 회원가입 창 그리기
void handleLogin(WINDOW *win);  // 로그인 처리 함수
void handleRegister(WINDOW *win);  // 회원가입 처리 함수

#endif