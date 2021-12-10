cmake_minimum_required(VERSION 3.10)
project(practicas-SO C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(SO
        p3.c
        processlist.c
        processlist.h
        common_utils.c
        common_utils.h
        )