cmake_minimum_required(VERSION 3.5)
project(GSP)

# Set C standard to C99
set(CMAKE_C_STANDARD 99)

#add_subdirectory(gWin)
add_subdirectory(gSerKit)

# Add executable target
#add_executable(GSP testing/main.c)

#target_link_libraries(GSP PRIVATE GWIN GVG)

#target_include_directories(GSP PRIVATE ./gWin/include)

add_executable(GSP testing/gSkTest.c)
target_link_libraries(GSP PRIVATE GSK)
target_include_directories(GSP PRIVATE ./gSerKit/include ./gVG/include/freetype2)