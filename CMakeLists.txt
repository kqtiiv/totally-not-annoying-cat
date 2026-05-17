cmake_minimum_required(VERSION 4.2)
project(totally_not_annoying_cat)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(totally_not_annoying_cat
        main.ino
        secrets.cpp)
