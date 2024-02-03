
cmake_minimum_required(VERSION 3.5)

project(GSP)

add_definitions(-w)

if (WIN32)

    message("Building GSP for Win32...")
    
    # add_subdirectory(System)

    add_executable(GSP ./Test/src/main.c)
else()
    message(FATAL_ERROR "Unsupported platform ${PLATFORM}")
endif()