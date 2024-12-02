#include <curses.h>
#include "board.h"
#include "loginGui.h"
#include "user.h"

/**
 *  "main.c"
 *  Produced by 김 준, 김준영
 *  Managed by 김준영
 */

int main() {
    initscr();
    FILE *fp = fopen("userInfo.txt", "r");
    loadAllMembers(fp);
    fclose(fp);
    FILE *viewBoardFP = fopen("board.txt", "r");
    loadAllBoards(viewBoardFP);
    fclose(viewBoardFP);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    handleLogin(stdscr);
    endwin();
    return 0;
}