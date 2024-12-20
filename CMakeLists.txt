cmake_minimum_required(VERSION 3.28)
project(protectedOS)

set(CMAKE_CXX_STANDARD 26)

add_executable(lab1
        lab1/main.c
        lab1/text.c
        lab1/text.h)
add_executable(mycat
        lab2_cat/main.c)
add_executable(mygrep
        lab2_grep/main.c)
add_executable(lab3
        lab3/main.c)
add_executable(lab4
        lab4/main.c)
add_executable(lab6_pipe
        lab6_pipe/main.c)
add_executable(lab6_fifo
        lab6_fifo/main.c)
add_executable(lab7_producer
        lab7/producer.c)
add_executable(lab7_consumer
        lab7/consumer.c)
add_executable(lab9_producer
        lab9/producer.c)
add_executable(lab9_consumer
        lab9/consumer.c)
add_executable(lab8
        lab8/main.c)
add_executable(lab11
        lab11/main.c)
add_executable(lab10
        lab10/main.c)
