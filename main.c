#include <curses.h>
#include "board.h"
#include "loginGui.h"
#include "user.h"

int main() {
    initscr();
    FILE *fp = fopen("userInfo.txt", "r");
    loadAllMembers(fp);
    fclose(fp);
    FILE *fp4 = fopen("group.txt", "r");
    loadGroup(fp4);
    fclose(fp4);
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