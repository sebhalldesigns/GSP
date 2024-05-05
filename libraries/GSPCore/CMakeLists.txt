cmake_minimum_required(VERSION 3.5)

message("Building OpenGSP GSPCore")


if (UNIX)

    message("Building OpenGSP GSPCore for UNIX/Linux")

    # Find X11 library
    find_package(X11 REQUIRED)
    

    set(GSPCORE_SRC
        src/platform/x11/GSPCore_x11.c
        src/platform/x11/GWindow_x11.c
        src/GApplication.c
        src/GVector.c
        src/GLog.c
        src/graphics/gl/GFrame_gl.c
        src/graphics/gl/GVertexBuffer_gl.c
        src/graphics/gl/GShader_gl.c
        src/graphics/gl/GTexture_gl.c
    )

    # Set the C compiler flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

    add_library(GSPCore STATIC ${GSPCORE_SRC})

    target_include_directories(GSPCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include  ${CMAKE_CURRENT_SOURCE_DIR})

    # Link X11 library
    target_link_libraries(GSPCore ${X11_LIBRARIES} GL GLEW m)
else()
    message("Unknown platform")
endif()

