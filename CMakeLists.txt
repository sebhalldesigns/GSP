cmake_minimum_required(VERSION 3.5)
project(Test)

# Set C standard to C99
set(CMAKE_C_STANDARD 99)
    # Set the C compiler flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")


add_compile_definitions(DEBUG)

add_subdirectory(libraries)


# Add executable target
add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/testing/main.c)
target_link_libraries(Test PRIVATE GSPCore)

target_include_directories(Test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/GSPCore/include)
