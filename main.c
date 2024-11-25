#include <ncurses.h>
#include "loginGui.h"

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    handleLogin(stdscr); // 로그인 화면에서 시작

    endwin();
    return 0;
}