cmake_minimum_required(VERSION 3.5)

if (LINUX)

    message("Building gWin for Linux")

    # Find X11 library
    find_package(X11 REQUIRED)
    
    set(GWIN_SRC
        x11/gWin.c
    )

    add_library(GWIN STATIC ${GWIN_SRC})

    target_include_directories(GWIN PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../gVG/include)

    # Link X11 library
    target_link_libraries(GWIN ${X11_LIBRARIES})

elseif(WIN32)

    message("Building gWin for Windows")
        
    set(GWIN_SRC
        win32/gWin.c
    )

    add_library(GWIN STATIC ${GWIN_SRC})

    target_include_directories(GWIN PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../gVG/include)

    # Link X11 library
    target_link_libraries(GWIN)

elseif(APPLE)

    message("Building gWin for macOS")

    find_library(COCOA_LIBRARY Cocoa REQUIRED)

    set(GWIN_SRC
        macOS/gWin.m
    )

    add_library(GWIN STATIC ${GWIN_SRC})

    target_include_directories(GWIN PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../gVG/include)

    target_link_libraries(GWIN ${COCOA_LIBRARY})

endif()