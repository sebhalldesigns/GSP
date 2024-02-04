
cmake_minimum_required(VERSION 3.7)

project(GSP)


find_package(Vulkan REQUIRED)

add_definitions(-w)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)

    message("Building GSP for Win32...")

    
    
    add_subdirectory(System)

    add_executable(GSP ./Test/src/main.cpp)
    target_link_libraries(GSP PRIVATE GSP-System)

    target_include_directories(GSP PRIVATE C:/VulkanSDK/1.3.250.0/Include ./System/include)
    target_link_libraries(GSP PUBLIC Vulkan::Vulkan)
else()
    message(FATAL_ERROR "Unsupported platform ${PLATFORM}")
endif()