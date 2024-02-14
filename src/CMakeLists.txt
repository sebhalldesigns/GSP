
cmake_minimum_required(VERSION 3.7)

project(GSP)

add_definitions(-w)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)

    message("Building GSP for Win32...")

    
    
    add_subdirectory(GSP-System)
    add_subdirectory(GSP-Graphics)

    #add_executable(GSP ./Test/src/main.cpp)
    #add_library(GSP STATIC ./Test/src/blank.cpp)
    #target_link_libraries(GSP PRIVATE GSP-System GSP-Graphics)

    #target_include_directories(GSP PRIVATE ./System/include ./Graphics/include)

    
else()
    message(FATAL_ERROR "Unsupported platform ${PLATFORM}")
endif()