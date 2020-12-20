cmake_minimum_required(VERSION 3.17)
project(cpp_arc C)

set(CMAKE_C_STANDARD 11)

add_executable(cpp_arc
        main.c types.c types.h utils.c utils.h constants.h archive.c archive.h)
