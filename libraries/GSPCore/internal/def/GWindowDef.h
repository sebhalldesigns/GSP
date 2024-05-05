#ifndef GWINDOWDEF_H
#define GWINDOWDEF_H

#include "GSPCore/GWindow.h"

typedef struct {
    void* rawHandle;
    int width;
    int height;
    const char* title;
    GWindowCloseDelegate closeDelegate;
    GWindowWillCloseDelegate willCloseDelegate;
    GWindowGraphicsInterface graphicsInterface;
    GWindowResizeDelegate resizeDelegate;
    GWindowPointerMoveDelegate pointerMoveDelegate;
    GWindowButtonDownDelegate buttonDownDelegate;
    GWindowButtonUpDelegate buttonUpDelegate;
    uintptr_t glContext;
    uintptr_t xDeleteAtom;
} GWindowDef;

void GWindowDef_Poll();
size_t GWindowDef_NumberOfOpenWindows();

#endif // GWINDOWDEF_H