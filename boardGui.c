#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include "boardGui.h"
#include "loginGui.h"
#include "list.h"
#include "user.h"
#include "board.h"


#define MAX_LEN 30
#define MAX_POSTS 5
#define MAX_MESSAGES 3




typedef struct {
    char sender[MAX_LEN];
    char message[256];
} Message;

// 초기화된 데이터
Message messages[MAX_MESSAGES] = { {"", ""}, {"", ""}, {"", ""} };


// 중앙 정렬을 위한 함수
void printCentered(WINDOW *win, int y, int width, const char *text) {
    int len = strlen(text);
    int x = (width - len) / 2; // 중앙 정렬을 위한 계산
    mvwprintw(win, y, x, "%s", text);
}

// 게시판 리스트 출력 함수
void showBoardList(WINDOW *win, int width) {
    int y = 2; // 출력 시작 위치
    node *temp = head; // 전역 변수로 선언된 게시판 리스트의 헤드 사용
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


// 게시물 상세보기 함수
void viewPost(WINDOW *win, int width) {
    mvwprintw(win, 2, 2, "Enter post number: ");
    char numStr[10];
    wgetstr(win, numStr);
    int postNum = atoi(numStr) - 1;

    if (postNum >= 0) {
        node *temp = head; // 리스트 탐색 시작
        int index = 0;
        while (temp != NULL) {
            if (temp->type == BOARD) {
                if (index == postNum) {
                    Board *board = (Board *) temp->data;
                    wclear(win);
                    box(win, 0, 0);
                    mvwprintw(win, 2, 2, "Post by: %s", board->writer);
                    mvwprintw(win, 3, 2, "Content: %s", board->contents);
                    mvwprintw(win, 5, 2, "Likes: %d | Hits: %d", board->likeCount, board->boardHit);
                    wrefresh(win);
                    wgetch(win); // 사용자 입력 대기
                    return;
                }
                index++;
            }
            temp = temp->next;
        }
    }

    mvwprintw(win, 7, 2, "Invalid post number.");
    wrefresh(win);
    wgetch(win); // 사용자 입력 대기
}

void addNewPost(WINDOW *win, int width, FILE *fp) {
    char writer[20], content[256];
    echo(); // 입력 표시 활성화
    mvwprintw(win, 2, 2, "Writer: ");
    wgetstr(win, writer);
    mvwprintw(win, 3, 2, "Content: ");
    wgetstr(win, content);
    noecho(); // 입력 표시 비활성화

    // 새 게시글 생성
    Board *newPost = initBoard(writer, content);
    if (newPost == NULL) {
        mvwprintw(win, 5, 2, "Failed to create new post.");
        wrefresh(win);
        wgetch(win);
        return;
    }

    // 메모리에 삽입
    insertBoard(newPost);

    // 파일에 저장
    if (fp != NULL) {
        saveBoard(newPost, fp); // `saveBoard`는 한 줄씩 저장
        fflush(fp); // 즉시 파일에 쓰기
        mvwprintw(win, 5, 2, "Post added and saved successfully!");
    } else {
        mvwprintw(win, 5, 2, "Post added, but file saving failed.");
    }

    wrefresh(win);
    wgetch(win); // 사용자 입력 대기
}

// 게시판 데이터 로드 함수 (파일에서 메모리로)
void loadBoardData(FILE *fp) {
    if (fp == NULL) {
        printf("Failed to open board file.\n");
        return;
    }

    loadAllBoards(fp); // 파일 데이터를 링크드 리스트에 로드
    printf("Board data loaded successfully.\n");
}

void handleGroupMenu(WINDOW *win);
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

    box(leftWin, 0, 0);
    box(rightWin, 0, 0);

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
                box(rightWin, 0, 0);
                if (selected == 0) {
                    handleGroupMenu(rightWin);
                }
                if (selected == 1) { // Board 메뉴
                    printCentered(rightWin, 1, rightWidth, "Board Posts:");
                    showBoardList(rightWin, rightWidth);

                    mvwprintw(rightWin, 20, 2, "Press 'n' to add new post, 'v' to view post, 'b' to go back.");
                    wrefresh(rightWin);

                    int boardCh;
                    while ((boardCh = wgetch(rightWin)) != 'b') { // 'b' 키로 복귀
                        if (boardCh == 'n') {
                            addNewPost(rightWin, rightWidth, fp);
                            wclear(rightWin);
                            box(rightWin, 0, 0);
                            printCentered(rightWin, 1, rightWidth, "Board Posts:");
                            showBoardList(rightWin, rightWidth);
                            mvwprintw(rightWin, 20, 2, "Press 'n' to add new post, 'v' to view post, 'b' to go back.");
                        } else if (boardCh == 'v') {
                            viewPost(rightWin, rightWidth);
                            wclear(rightWin);
                            box(rightWin, 0, 0);
                            printCentered(rightWin, 1, rightWidth, "Board Posts:");
                            showBoardList(rightWin, rightWidth);
                            mvwprintw(rightWin, 20, 2, "Press 'n' to add new post, 'v' to view post, 'b' to go back.");
                        }
                    }
                } else if (selected == 3) { // Logout
                    clear();
                    refresh();
                    fclose(fp); // 파일 닫기
                    handleLogin(win);
                    return;
                } else if (selected == 4) { // Exit
                    fclose(fp); // 파일 닫기
                    endwin();
                    return;
                }

                wrefresh(rightWin);
                focusBox = 0; // 다시 메뉴로 복귀
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

    fclose(fp); // 파일 닫기
    endwin();
}
void createNewGroup(WINDOW *win) {
    char groupName[MAX_LEN], leaderName[MAX_LEN];
    echo();
    mvwprintw(win, 2, 2, "Enter Group Name: ");
    wgetstr(win, groupName);
    mvwprintw(win, 3, 2, "Enter Leader Name: ");
    wgetstr(win, leaderName);
    noecho();

    if (1) {
        group *group =createGroup(groupName, leaderName);
        FILE *groupFp = fopen("group.txt", "a+");
        saveGroup(groupFp,group);
        fclose(groupFp);
        mvwprintw(win, 5, 2, "Group '%s' created successfully.", groupName);
    } else {
        mvwprintw(win, 5, 2, "Failed to create group '%s'.", groupName);
    }

    wrefresh(win);
    wgetch(win);
}

void joinExistingGroup(WINDOW *win) {
    char groupName[MAX_LEN], userName[MAX_LEN];
    echo();
    mvwprintw(win, 2, 2, "Enter Group Name: ");
    wgetstr(win, groupName);
    mvwprintw(win, 3, 2, "Enter Your Username: ");
    wgetstr(win, userName);
    noecho();

    // 그룹 검색
    group *existingGroup = findGroup(groupName);
    if (existingGroup == NULL) {
        mvwprintw(win, 5, 2, "Group '%s' not found.", groupName);
        wrefresh(win);
        wgetch(win); // 사용자 입력 대기
        return;
    }

    // 그룹에 가입
    joinGroup(groupName, userName);

    // 그룹 저장
    FILE *groupFp2 = fopen("group.txt", "a");
    if (groupFp2 == NULL) {
        mvwprintw(win, 5, 2, "Failed to open group file for writing.");
        wrefresh(win);
        wgetch(win); // 사용자 입력 대기
        return;
    }

    saveGroup(groupFp2, existingGroup);
    fclose(groupFp2);

    mvwprintw(win, 5, 2, "Successfully joined group '%s'.", groupName);
    wrefresh(win);
    wgetch(win); // 사용자 입력 대기
}

void viewGroupMembers(WINDOW *win) {
    char groupName[MAX_LEN];
    echo();
    mvwprintw(win, 2, 2, "Enter Group Name: ");
    wgetstr(win, groupName);
    noecho();

    wclear(win);
    box(win, 0, 0);
    FILE *viewGroupFp = fopen("group.txt", "r");
    loadGroup(viewGroupFp);
    fclose(viewGroupFp);
    showGroupMembers(groupName);
    mvwprintw(win, 2, 2, "Members of group '%s' are displayed above.", groupName);
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
        // 메뉴 표시
        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "Group Menu:");

        for (int i = 0; i < 4; i++) {
            if (i == selected) {
                wattron(win, A_REVERSE); // 선택된 항목 강조
                mvwprintw(win, 3 + i, 2, "%s", options[i]);
                wattroff(win, A_REVERSE);
            } else {
                mvwprintw(win, 3 + i, 2, "%s", options[i]);
            }
        }

        wrefresh(win);

        ch = wgetch(win); // 사용자 입력 대기

        // 메뉴 선택
        if (ch == 'w' || ch == 'W') {
            selected = (selected == 0) ? 3 : selected - 1;
        } else if (ch == 's' || ch == 'S') {
            selected = (selected == 3) ? 0 : selected + 1;
        } else if (ch == 10) { // Enter 키
            if (selected == 0) {
                // Create Group
                createNewGroup(win);
            } else if (selected == 1) {
                // Join Group
                joinExistingGroup(win);
            } else if (selected == 2) {
                // View Group Members
                viewGroupMembers(win);
            } else if (selected == 3) {
                // Back
                return;
            }
        }
    }
}