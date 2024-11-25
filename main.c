#include <ncurses.h>
#include "loginGui.h"
#include "user.h"

int main() {
    // FILE *fp2 = fopen("userInfo.txt","a");
    // userRegister(fp2,"aa","bb","cc");

    // fclose(fp);
    // fclose(fp2);
    initscr();
    // FILE *fp = fopen("userInfo.txt", "r");
    // loadMember(fp);
    // fclose(fp);
    //
    // UserInfo *user = findUser("a");
    // printf("\n%s\n", user->nickname);
    // int a = userLogin("b","c");
    // printf("%d\n", a);
    // int b = userRegister(fp2,"bb","cc","dd");
    // printf("b=%d\n", b);
    // printf("login = %d\n",userLogin("cc","dd"));
    // fclose(fp2);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    handleLogin(stdscr); // 로그인 화면에서 시작

    endwin();
    return 0;
}