cmake_minimum_required(VERSION 3.5)
project(chat)

find_package(Threads)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")

set(LIB_FILES
        lib/commons.c
        lib/commons.h
        lib/interfaces.c
        lib/interfaces.h)

add_executable(client client_new.c ${LIB_FILES})
add_executable(server server.c
        lib/message_queue.c
        lib/message_queue.h
        lib/client_list.c
        lib/client_list.h
        lib/executors.c
        lib/executors.h
        ${LIB_FILES})

target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
