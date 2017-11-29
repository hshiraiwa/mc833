cmake_minimum_required(VERSION 3.5)
project(chat)

find_package(Threads)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")

add_executable(client
        client.c
	lib/client/executors.c
	lib/client/executors.h
	lib/client/file_transfer.c
	lib/client/file_transfer.h
	lib/client/utils.c
	lib/client/utils.h
	lib/commons.c
	lib/commons.h
	lib/interfaces.c
	lib/interfaces.h
	)

add_executable(server
        server.c
	lib/server/client_list.c
	lib/server/client_list.h
	lib/server/executors.c
	lib/server/executors.h
	lib/server/message_queue.c
	lib/server/message_queue.h
	lib/commons.c
	lib/commons.h
	lib/interfaces.c
	lib/interfaces.h
        )

target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})
