cmake_minimum_required(VERSION 3.29)
project(CteamProject C)

set(CMAKE_C_STANDARD 11)

add_executable(CteamProject main.c
        list.c
        board.h
        board.c)
