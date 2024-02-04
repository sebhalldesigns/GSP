
if (WIN32)

    message("Building GSP-System for Win32...")

    set(GSP_GRAPHICS_SOURCE
        src/Win32/GspGraphics.cpp
        src/Win32/gOpenGL.cpp
    )

    
    find_package(OpenGL REQUIRED)

    set(GSP_GRAPHICS_LIBRARIES 
    OpenGL::GL
    )

    add_library(GSP-Graphics STATIC ${GSP_GRAPHICS_SOURCE})

    target_include_directories(GSP-Graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ ${PROJECT_SOURCE_DIR}/System/include/)

    target_link_libraries(GSP-Graphics PUBLIC ${GSP_GRAPHICS_LIBRARIES})

endif()