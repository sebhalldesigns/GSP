cmake_minimum_required(VERSION 3.5)

if (LINUX)

    message(Building gWin for Linux)
    
    set(GWIN_SRC
        gWin.c
        x11/gWin_x11.c
    )

    # Find X11 library
    find_package(X11 REQUIRED)

    add_library(GWIN STATIC ${GWIN_SRC})

    target_include_directories(GWIN PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    # Link X11 library
    target_link_libraries(GWIN ${X11_LIBRARIES})

endif()