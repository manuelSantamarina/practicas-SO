cmake_minimum_required(VERSION 3.20)
project(practicas-SO C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(SO p0.c common_utils.c libp3.c)