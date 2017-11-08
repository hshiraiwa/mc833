cmake_minimum_required(VERSION 3.5)
project(chat)

set(CMAKE_CXX_STANDARD 11)

set(LIB_FILES
        lib/commons.c
        lib/commons.h
        lib/interfaces.c
        lib/interfaces.h)

add_executable(client client_new.c ${LIB_FILES})
add_executable(server server.c ${LIB_FILES})
