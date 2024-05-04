#include "GSPCore/GSPCore.h"
#include "GSPCore/GWindow.h"
#include "GSPCore/GLog.h"

#include <stdio.h>

GWindowSize WindowWillResize(GWindow window, GWindowSize proposedSize) {
    printf("window will resize to %d %d \n", proposedSize.width, proposedSize.height);

    int xrem = proposedSize.width % 100;
    int yrem = proposedSize.height % 100;

    proposedSize.width -= xrem;
    proposedSize.height -= yrem;


    printf("setting to %d %d \n", proposedSize.width, proposedSize.height);
    return proposedSize;
}

void WindowResized(GWindow window, GWindowSize newSize) {
    printf("window resized to %d %d \n", newSize.width, newSize.height);
}

typedef struct {
    float a;
    int b;
    const char* c;
} exampleStruct;

void LaunchEvent() {
    printf("Launched!\n");
    GLog(INFO, "info");
    GLog(WARNING, "warning");
    GLog(ERROR, "warning");
    GLog(FAILURE, "warning");

    GWindowInfo info = {
        800,
        600,
        "My Window"
    };

    GWindow window = GWindow_Init(info);
    GWindow_SetResizeDelegate(window, WindowResized);
    GWindow_SetWillResizeDelegate(window, WindowWillResize);
}

int main() {

    GApplicationInfo info = {
        "example app", 
        "Seb Hall", 
        0, 0
    };

    GApplication app = GApplication_Init(info);
    
    GApplication_SetLaunchEvent(app, LaunchEvent);

    return GSPRun(app);
}