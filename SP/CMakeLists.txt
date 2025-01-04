cmake_minimum_required(VERSION 3.30)
project(SP C)

set(CMAKE_C_STANDARD 11)
add_executable(SP main.c
        client.c
        client.h
        server.c
        server.h
        config.h)
target_link_libraries(SP PRIVATE ws2_32)