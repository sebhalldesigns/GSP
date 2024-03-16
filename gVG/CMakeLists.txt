cmake_minimum_required(VERSION 3.5)

message("Building gVG")

set(GVG_SRC
    gVG.c
)

add_library(GVG STATIC ${GVG_SRC})

target_include_directories(GVG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

