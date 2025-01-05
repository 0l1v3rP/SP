cmake_minimum_required(VERSION 3.22.1)
project(SP C)

set(CMAKE_C_STANDARD 11)
add_executable(SP main.c
        client.c
        client.h
        server.c
        server.h
        config.h
        table.c
        table.h
)