#ifndef GWINDOWDEF_H
#define GWINDOWDEF_H

#include "GSPCore/GWindow.h"

typedef struct {
    void* rawHandle;
    int width;
    int height;
    const char* title;
    GWindowResizeEvent resizeEvent;
} GWindowDef;

void GWindowDef_Poll();

#endif // GWINDOWDEF_H