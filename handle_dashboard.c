#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include "handle_dashboard.h"
#include "login_register.h"

#define MAX_LEN 30
#define MAX_POSTS 5
#define MAX_MESSAGES 3

// 구조체 정의
typedef struct {
    char groupName[MAX_LEN];  // 그룹 이름
    char members[5][MAX_LEN]; // 그룹 멤버들 (최대 5명)
} Group;

typedef struct {
    char boardTitle[MAX_LEN];
    char boardContent[256];
} Post;

typedef struct {
    char sender[MAX_LEN];
    char message[256];
} Message;

// 초기화된 데이터
Group userGroup = {"Group", {"user1", "user2", "user3", "", ""}};
Post posts[MAX_POSTS] = { {"First Post", "This is the content of the first post."}, {"Second Post", "Content of second post."} };
Message messages[MAX_MESSAGES] = { {"", ""}, {"", ""}, {"", ""} };

// 중앙 정렬을 위한 함수
void printCentered(WINDOW *win, int y, int width, const char *text) {
    int len = strlen(text);
    int x = (width - len) / 2; // 중앙 정렬을 위한 계산
    mvwprintw(win, y, x, "%s", text);
}

// 대시보드 화면을 그리는 함수
void handleDashboard(WINDOW *win) {
    clear();
    refresh();

    int dashboardHeight = 25, dashboardWidth = 50;
    int dashboardStartY = 2, dashboardStartX = 10;  // 대시보드의 시작 위치

    int leftWidth = 20, rightWidth = 80;  // 왼쪽 박스와 오른쪽 박스의 너비
    int totalWidth = leftWidth + rightWidth;
    int totalHeight = dashboardHeight;

    WINDOW *leftWin = newwin(totalHeight, leftWidth, dashboardStartY, dashboardStartX);
    WINDOW *rightWin = newwin(totalHeight, rightWidth, dashboardStartY, dashboardStartX + leftWidth);

    box(leftWin, 0, 0);
    box(rightWin, 0, 0);

    // 왼쪽 박스의 항목들 표시
    printCentered(leftWin, 1, leftWidth, "BLOG");
    printCentered(leftWin, 3, leftWidth, "*admin*"); // 내 닉네임 표시
    printCentered(leftWin, 4, leftWidth, userGroup.groupName); // 내 그룹 표시
    printCentered(leftWin, 6, leftWidth, "Posts");
    for (int i = 0; i < 2; i++) {
        char postStr[MAX_LEN + 10];
        snprintf(postStr, sizeof(postStr), "%d. %s", i + 1, posts[i].boardTitle);
        printCentered(leftWin, 7 + i, leftWidth, postStr);
    }

    // 메뉴 항목들
    char *menu[] = {
        "Group",
        "Board",
        "Messages",
        "Logout",  // 로그아웃 버튼 추가
        "Exit"     // 종료 버튼 추가
    };

    // 왼쪽 박스의 메뉴 항목들 표시
    for (int i = 0; i < 5; i++) {
        printCentered(leftWin, 12 + i, leftWidth, menu[i]);
    }

    // 왼쪽 박스에 포커스를 맞춘 항목에 처음부터 강조 표시
    wattron(leftWin, A_REVERSE);  // 처음에 "Group"에 하이라이트를 적용
    printCentered(leftWin, 12 + 0, leftWidth, menu[0]);
    wattroff(leftWin, A_REVERSE);

    wrefresh(leftWin);
    wrefresh(rightWin);

    // 사용자 입력 처리
    int selected = 0;  // 왼쪽 박스에서 선택된 항목 (처음부터 0으로 설정)
    int focusBox = 0;  // 0: 왼쪽 박스, 1: 오른쪽 박스
    int ch;

    while ((ch = wgetch(leftWin)) != 'q') {
        // 왼쪽 박스에서 포커스 이동
        if (focusBox == 0) {
            if (ch == 'w' || ch == 'W') {  // 위로 이동 (왼쪽 박스에서)
                selected = (selected == 0) ? 4 : selected - 1;  // 위로 이동
            } else if (ch == 's' || ch == 'S') {  // 아래로 이동 (왼쪽 박스에서)
                selected = (selected == 4) ? 0 : selected + 1;  // 아래로 이동
            } else if (ch == 10) {  // 엔터키 처리 (오른쪽 창으로 이동)
                focusBox = 1;
                wclear(rightWin);  // 오른쪽 창 초기화
                box(rightWin, 0, 0);  // 오른쪽 창 다시 그리기
                wrefresh(rightWin);  // 오른쪽 창 갱신

                // 왼쪽 박스에서 선택된 항목에 따라 오른쪽 창에 내용 표시
                if (selected == 0) {
                    printCentered(rightWin, 1, rightWidth, "Group: ");
                    printCentered(rightWin, 2, rightWidth, userGroup.groupName);
                    printCentered(rightWin, 3, rightWidth, "Create Group or Join?");
                } else if (selected == 1) {
                    printCentered(rightWin, 1, rightWidth, "Board Posts:");
                    for (int i = 0; i < 2; i++) {
                        char postStr[MAX_LEN + 10];
                        snprintf(postStr, sizeof(postStr), "%d. %s", i + 1, posts[i].boardTitle);
                        printCentered(rightWin, 2 + i, rightWidth, postStr);
                    }
                    printCentered(rightWin, 5, rightWidth, "new post.");
                } else if (selected == 2) {
                    printCentered(rightWin, 1, rightWidth, "Messages:");
                    for (int i = 0; i < 3; i++) {
                        char messageStr[MAX_LEN + 100];
                        snprintf(messageStr, sizeof(messageStr), "%s: %s", messages[i].sender, messages[i].message);
                        printCentered(rightWin, 2 + i, rightWidth, messageStr);
                    }
                    printCentered(rightWin, 6, rightWidth, "Press 'r' to reply.");
                } else if (selected == 3) {
                    // 로그아웃 처리
                    clear();
                    refresh();
                    handleLogin(win); // 로그인 화면으로 돌아가기
                    return;
                } else if (selected == 4) {
                    // 종료 처리
                    endwin();
                    return;
                }

                wrefresh(rightWin);  // 오른쪽 창 갱신
            }
        }

        // 오른쪽 창에서 선택 후 왼쪽 창으로 돌아가게 할 부분 (b키 처리)
        if (focusBox == 1) {
            if (ch == 'b') {  // b키 눌렀을 때 왼쪽 창에서 계속 선택하도록 수정
                // 오른쪽 창에서 b를 눌렀을 때 왼쪽 창에서 다시 선택하도록 처리
                focusBox = 0;
                wclear(rightWin);  // 오른쪽 창 초기화
                box(rightWin, 0, 0);  // 오른쪽 창 다시 그리기
                wrefresh(rightWin);  // 오른쪽 창 갱신
            }
        }

        // 왼쪽 박스에 포커스를 맞춘 항목에 강조 표시
        for (int i = 0; i < 5; i++) {
            if (i == selected && focusBox == 0) {
                wattron(leftWin, A_REVERSE);  // 포커스된 항목 강조
                printCentered(leftWin, 12 + i, leftWidth, menu[i]);
                wattroff(leftWin, A_REVERSE);  // 강조 해제
            } else {
                printCentered(leftWin, 12 + i, leftWidth, menu[i]);
            }
        }

        wrefresh(leftWin);
        wrefresh(rightWin);
    }

    endwin();
}
