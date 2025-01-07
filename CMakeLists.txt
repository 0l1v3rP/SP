cmake_minimum_required(VERSION 3.22.1)
project(SP C)

set(CMAKE_C_STANDARD 11)
add_executable(SP
        src/server.c
        src/main.c
        src/client.c
        src/table.c
        headers/client.h
        headers/config.h
        headers/server.h
        headers/table.h
)