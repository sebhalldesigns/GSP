cmake_minimum_required(VERSION 3.5)

message("Building gVG")

set(GVG_SRC
    gVG.c
)

if (WIN32) 

    set(FREETYPE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/Lib/windows-x64/freetype.lib)

endif()

add_library(GVG STATIC ${GVG_SRC})

target_include_directories(GVG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(GVG PUBLIC ${FREETYPE_LIB})

