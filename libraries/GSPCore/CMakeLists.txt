cmake_minimum_required(VERSION 3.5)

message("Building OpenGSP GSPCore")


set(GSPCORE_SRC
    src/GSPCore.c
    src/GApplication.c
    
)



add_library(GSPCore STATIC ${GSPCORE_SRC})

target_include_directories(GSPCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include  ${CMAKE_CURRENT_SOURCE_DIR})