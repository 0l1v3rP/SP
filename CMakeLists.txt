cmake_minimum_required(VERSION 3.22.1)
project(SP C)

set(CMAKE_C_STANDARD 11)
add_executable(SP
        src/server/server.c
        src/main.c
        src/client/client.c
        src/server/data/table.c
        headers/client/client.h
        headers/config.h
        headers/server/server.h
        headers/server/data/table.h
)