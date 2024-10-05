cmake_minimum_required(VERSION 3.28)
project(protectedOS)

set(CMAKE_CXX_STANDARD 26)

add_executable(lab1 lab1/main.c
        lab1/text.c
        lab1/text.h)
