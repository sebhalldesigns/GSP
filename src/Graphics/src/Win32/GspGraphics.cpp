#include "GspGraphics.h"
#include "gOpenGL.h"

#include <cstdio>
#include <Windows.h>

static gGraphicsApi selectedApi = gGraphicsApi::NO_API;

//  GspGraphicsInit - global init for graphics - should only be called once by system
gResult GspGraphicsInit() {

    printf("Starting GSP Graphics...\n");

    printf("Searching for Vulkan...");

    HMODULE vulkanModule = LoadLibrary("vulkan-1.dll");

    if (vulkanModule != NULL) {
        printf("found\nUsing Vulkan for rendering.\n");
        selectedApi = OPENGL;
    } else {
        printf("not found\nUsing legacy OpenGL for rendering.\n");
        selectedApi = OPENGL;
    }

    return gResult::SUCCESS;
}

//  GspGraphicsGetApi - identify which API is being used
gGraphicsApi GspGraphicsGetApi() {
    return selectedApi;
}


//  GspGraphicsInitForWindow - setup and allocate resources for rendering a window.
//  Should only be called once per window by the system.
gResult GspGraphicsInitForWindow(gWindow window) {
    switch (selectedApi) {
        case OPENGL: {
            return gOpenGLInitForWindow(window);
        }

        default: {
            return gResult::WARNING;
        }
    }
}

//  GspGraphicsCloseForWindow - deallocate rendering resources for a window
//  Should only be called when a window is deallocated
gResult GspGraphicsCloseForWindow(gWindow window) {
    switch (selectedApi) {
        case OPENGL: {
            return gOpenGLCloseForWindow(window);
        }

        default: {
            return gResult::WARNING;
        }
    }
}

//  GspGraphicsStartRender - start rendering frame, called by system only
gResult GspGraphicsStartRender(gWindow window) {
    switch (selectedApi) {
        case OPENGL: {
            return gOpenGLStartRender(window);
        }

        default: {
            return gResult::WARNING;
        }
    }
}

//  GspGraphicsCommitRender - stop rendering frame and swap buffers, called by system only
gResult GspGraphicsCommitRender(gWindow window) {
    switch (selectedApi) {
        case OPENGL: {
            return gOpenGLCommitRender(window);
        }

        default: {
            return gResult::WARNING;
        }
    }
}

//  GspGraphicsFillRect - fill rect on active window
gResult GspGraphicsFillRect(gRect rect, gColor color) {
    switch (selectedApi) {
        case OPENGL: {
            return gOpenGLFillRect(rect, color);
        }

        default: {
            return gResult::WARNING;
        }
    }
}
