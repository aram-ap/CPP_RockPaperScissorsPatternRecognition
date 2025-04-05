cmake_minimum_required(VERSION 3.30)
project(CPP_RockPaperScissorsML)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(CPP_RockPaperScissorsML main.cpp
        main.h)
