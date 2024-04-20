cmake_minimum_required(VERSION 3.5)

project(gsp_simple)

# Find Rust library
find_library(RUST_LIBRARY gsp_system
    HINTS ${CMAKE_SOURCE_DIR}/../../gsp_system/target/debug
          ${CMAKE_SOURCE_DIR}/../../gsp_system/target/release
)

# Add C executable
add_executable(gsp_simple main.c)

# Include Rust library headers if any
target_include_directories(gsp_simple PUBLIC ${CMAKE_SOURCE_DIR}/../../gsp_system/include/)

if(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa)
    target_link_libraries(gsp_simple ${RUST_LIBRARY} ObjC ${COCOA_FRAMEWORK})
elseif(WIN32)
    target_link_libraries(gsp_simple ${RUST_LIBRARY} kernel32.lib advapi32.lib userenv.lib kernel32.lib kernel32.lib ws2_32.lib bcrypt.lib ntdll.lib msvcrt.lib legacy_stdio_definitions.lib)
endif() 
