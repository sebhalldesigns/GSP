#ifndef GWINDOWDEF_H
#define GWINDOWDEF_H

#include "GSPCore/GWindow.h"

typedef struct {
    void* rawHandle;
    int width;
    int height;
    const char* title;
    GWindowDrawDelegate drawDelegate;
    GWindowCloseDelegate closeDelegate;
    GWindowWillCloseDelegate willCloseDelegate;
    GWindowGraphicsInterface graphicsInterface;
    GWindowResizeDelegate resizeDelegate;
    GWindowPointerMoveDelegate pointerMoveDelegate;
    GWindowButtonDownDelegate buttonDownDelegate;
    GWindowButtonUpDelegate buttonUpDelegate;
    uintptr_t glContext;
    uintptr_t xDeleteAtom;
    uint32_t simpleVAO;
    uint32_t simpleShader;
    uint32_t texture;
} GWindowDef;

void GWindowDef_Poll();
size_t GWindowDef_NumberOfOpenWindows();

#endif // GWINDOWDEF_H