cmake_minimum_required(VERSION 3.29)
project(CteamProject C)

set(CMAKE_C_STANDARD 11)

# libcurl 찾기
find_package(CURL REQUIRED)

# ncurses 찾기
find_package(Curses REQUIRED)

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

# libcurl 및 ncurses 링크
target_link_libraries(CteamProject CURL::libcurl ${CURSES_LIBRARIES})