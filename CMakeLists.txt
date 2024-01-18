cmake_minimum_required(VERSION 3.27)
project(lexer C)

set(CMAKE_C_STANDARD 11)

add_executable(lexer main.c
        lex.h
        lex.c)
