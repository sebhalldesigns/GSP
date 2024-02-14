#include "gOpenGL.h"

#include <Windows.h>
#include <gl/GL.h>
#include <gl/glu.h>
#include <map>

#include <cstdio>

struct gOpenGLInternalInfo {
    HGLRC glRenderContextHandle;
};

static HDC workingDeviceContext;
static PAINTSTRUCT workingPaintStruct;
static gSize workingWindowSize;

static std::map<gWindow, gOpenGLInternalInfo> graphicsHandleMap;

gResult gOpenGLInitForWindow(gWindow window) {

    printf("SETUP FOR WINDOW");

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24, // bit depth
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        16, // depth buffer
        0, // stencil buffer
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    HDC hdc = GetDC((HWND)window);

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    graphicsHandleMap[window] = {};
    graphicsHandleMap[window].glRenderContextHandle = wglCreateContext(hdc);
    
    wglMakeCurrent(hdc, graphicsHandleMap[window].glRenderContextHandle);
    
    gSize size;
    gGetWindowSize(window, size);
    glViewport(0, 0, (int)size.width, (int)size.height);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glEnable(GL_TEXTURE_2D);   
    glEnable(GL_POINT_SMOOTH); 
    glEnable(GL_SCISSOR_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction for text

    ReleaseDC((HWND)window, hdc);

    return gResult::SUCCESS;
}

gResult gOpenGLCloseForWindow(gWindow window) {

    // check window is in graphics map
    if (graphicsHandleMap.find(window) != graphicsHandleMap.end()) {
        wglDeleteContext(graphicsHandleMap[window].glRenderContextHandle); 
    }

    graphicsHandleMap.erase(window);

    return gResult::SUCCESS;  
}

gResult gOpenGLStartRender(gWindow window) {
    // check window is in graphics map
    if (graphicsHandleMap.find(window) == graphicsHandleMap.end()) {
        return gResult::ERR;
    }

    workingDeviceContext = BeginPaint((HWND)window, &workingPaintStruct);

    wglMakeCurrent(workingDeviceContext, graphicsHandleMap[window].glRenderContextHandle);

    gGetWindowSize(window, workingWindowSize);
    glViewport(0, 0, (int)workingWindowSize.width, (int)workingWindowSize.height);
    glScissor(0, 0, (int)workingWindowSize.width, (int)workingWindowSize.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, workingWindowSize.width, workingWindowSize.height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear window to transparent
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    return gResult::SUCCESS;  
}

gResult gOpenGLCommitRender(gWindow window) {
    // check window is in graphics map
    if (graphicsHandleMap.find(window) == graphicsHandleMap.end()) {
        return gResult::ERR;
    }

    SwapBuffers(workingDeviceContext);
    EndPaint((HWND)window, &workingPaintStruct);
}

gResult gOpenGLFillRect(gRect rect, gColor color) {
    glScissor(rect.x, workingWindowSize.height - rect.y - rect.height, rect.width, rect.height);

    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
    return gResult::SUCCESS;  
}