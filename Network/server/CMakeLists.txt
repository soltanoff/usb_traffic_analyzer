cmake_minimum_required(VERSION 2.0)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
    project(client)
    set(SOURCE_FILES
            main.cpp
            src/client/client.cpp
            src/config/config.cpp)

    add_executable(client ${SOURCE_FILES})
    target_link_libraries(client wsock32 ws2_32)
else()
    project(server)
    set(SOURCE_FILES
            main.cpp
            src/server/server.cpp
            src/client/client.cpp
            src/core/clientinteraction.cpp
            src/config/config.cpp
            src/core/commands/functions.cpp
            src/core/commands/scheduler.cpp
            src/core/database/database.cpp
            src/core/commands/functions.h)

    add_definitions(-std=c++14 -O0 -g)
    add_executable(server ${SOURCE_FILES})
    target_link_libraries(server pthread mysqlclient)
endif()
