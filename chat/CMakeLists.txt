cmake_minimum_required(VERSION 3.5)
project(chat)

find_package(Threads)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")

file(GLOB LIB_FILES "lib/*.c" "lib/*.h")
file(GLOB SERVER_LIB "lib/server/*.c" "lib/server/*.h")
file(GLOB CLIENT_LIB "lib/client/*.c" "lib/client/*.h")

add_executable(client
        client_new.c
        ${CLIENT_LIB}
        ${LIB_FILES})

add_executable(server
        server.c
        ${SERVER_LIB}
        ${LIB_FILES})

target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
