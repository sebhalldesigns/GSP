cmake_minimum_required(VERSION 3.5)

# message("Building gVG")



if (WIN32) 

    set(FREETYPE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/windows-x64/freetype.lib)

    set(GVG_SRC
        gVG.c
    )

elseif(LINUX)
    message("Building OpenGSP gVG for Linux")

    set(FREETYPE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux-x86-64/libfreetype.a)

    set(GVG_SRC
        gVG.c
        posix/gVG_kernel.c
        posix/gVG_thread.c
        pipeline/gvg_common.c
        pipeline/gvg_bytegen.c
        pipeline/gvg_combine.c
    )

endif()


add_library(GVG STATIC ${GVG_SRC})

target_include_directories(GVG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(GVG PUBLIC ${FREETYPE_LIB} m)
