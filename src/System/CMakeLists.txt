
if (WIN32)

    message("Building GSP-System for Win32...")

    set(GSP_SYSTEM_SOURCE
        src/Win32/GspSystem.cpp
    )

    set(GSP_SYSTEM_LIBRARIES 
    )

    add_library(GSP-System STATIC ${GSP_SYSTEM_SOURCE})

    target_include_directories(GSP-System PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ ${PROJECT_SOURCE_DIR}/Graphics/include/)

    target_link_libraries(GSP-System PUBLIC ${GSP_SYSTEM_LIBRARIES})

endif()