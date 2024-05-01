cmake_minimum_required(VERSION 3.5)

message("Building OpenGSP gSerKit")

set(GSK_SRC
    gSkXml.c
)


add_library(GSK STATIC ${GSK_SRC})

target_include_directories(GSK PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
