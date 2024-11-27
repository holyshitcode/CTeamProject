cmake_minimum_required(VERSION 3.29)
project(CteamProject C)

set(CMAKE_C_STANDARD 11)

# 소스 파일 추가 및 실행 파일 정의
add_executable(CteamProject
        main.c
        list.c
        board.c
        message.h
        message.c
        user.h
        user.c
        boardGui.c
        boardGui.h
        loginGui.c
        loginGui.h
)

include_directories(${CMAKE_SOURCE_DIR}/PDCurses)

target_link_libraries(CteamProject ${CMAKE_SOURCE_DIR}/PDCurses/wincon/pdcurses.a
        kernel32 user32 gdi32 winspool shell32 ole32 oleaut32 uuid comdlg32 advapi32)