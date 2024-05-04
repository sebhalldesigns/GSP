#ifndef GWINDOW_H
#define GWINDOW_H

#include <stddef.h>

typedef void* GWindow;

typedef struct {
    int width;
    int height;
    const char* title;
} GWindowInfo;

typedef struct {
    int width;
    int height;
} GWindowSize;

typedef void(*GWindowResizeDelegate)(GWindow window, GWindowSize size);
typedef GWindowSize(*GWindowWillResizeDelegate)(GWindow window, GWindowSize proposedSize);

GWindow GWindow_Init(GWindowInfo info);

// connect delegates after creation
void GWindow_SetResizeDelegate(GWindow window, GWindowResizeDelegate resizeDelegate);
void GWindow_SetWillResizeDelegate(GWindow window, GWindowWillResizeDelegate willResizeDelegate);


#endif // GWINDOW_H