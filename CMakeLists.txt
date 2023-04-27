cmake_minimum_required(VERSION 3.21)
project(task5_chiklina215 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(task5_chiklina215
        main.c
        word.c
        tree.c
        list.c
        handler.c
        )
