#ifndef GWINDOW_H
#define GWINDOW_H

#include <stddef.h>

typedef void* GWindow;

typedef struct {
    int width;
    int height;
    const char* title;
} GWindowInfo;

typedef void(*GWindowResizeEvent)();

GWindow GWindow_Init(GWindowInfo info);

// connect events after creations
void GWindow_SetResizeEvent(GWindow window, GWindowResizeEvent resizeEvent);


#endif // GWINDOW_H