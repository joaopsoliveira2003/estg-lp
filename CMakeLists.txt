cmake_minimum_required(VERSION 3.23)
project(estg_lp C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(estg_lp
        administrador.c
        clientes.c
        colors.c
        data.c
        definitions.h
        main.c
        queries.c
        shopping.c
        util.c)
