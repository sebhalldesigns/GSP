cmake_minimum_required(VERSION 3.5)
project(GSP)

# Set C standard to C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")



#add_subdirectory(gWin)
#add_subdirectory(gSerKit)

# Add executable target
#add_executable(GSP MACOSX_BUNDLE testing/WindowTest.c)
#set_target_properties(GSP PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/gBuild/files/macOS/minimal.plist)

#target_link_libraries(GSP PRIVATE GWIN)

#target_include_directories(GSP PRIVATE ./gWin/include)

#add_executable(GSP testing/gSkTest.c)
#target_link_libraries(GSP PRIVATE GSK)
#target_include_directories(GSP PRIVATE ./gSerKit/include ./gVG/include/freetype2)

add_executable(GSP testing/emcc_test.c)
target_link_libraries(GSP PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/testing/libfreetype.a)
target_include_directories(GSP PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gVG/include/freetype2)

# Set Emscripten specific options
set(CMAKE_EXECUTABLE_SUFFIX ".js")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s USE_WEBGL2=1")
# Specify output directory for HTML files
set_target_properties(GSP PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html"

)

set(HTML_FILE "gBuild/files/web/index.html")
configure_file(${HTML_FILE} ${CMAKE_CURRENT_BINARY_DIR}/html COPYONLY)