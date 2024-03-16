cmake_minimum_required(VERSION 3.26)
project(SO2_202003585 C)

set(CMAKE_C_STANDARD 11)

include_directories(Practica2/src/include)

add_executable(SO2_202003585
        Practica2/src/include/data_loader.h
        Practica2/src/include/menus.h
        Practica2/src/include/structures.h
        Practica2/src/include/operations.h
        Practica2/src/data_loader.c
        Practica2/src/main.c
        Practica2/src/menus.c
        Practica2/src/operations.c
)
