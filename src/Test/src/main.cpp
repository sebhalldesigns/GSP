// GSP test program

#include "GspSystem.h"
#include "GspGraphics.h"

#include <cstdio>

void WindowResized(gWindow window, gSize size) {
    //printf("WINDOW RESIZED %f %f\n", size.width, size.height);
}

void WindowMoved(gWindow window, gPoint point) {
    //printf("WINDOW MOVED %f %f\n", point.x, point.y);
}

void WindowDraw(gWindow window) {

    gSize size;
    gGetWindowSize(window, size);

    GspGraphicsFillRect({0.0, 0.0, size.width/2, size.height}, {1.0, 0.0, 0.0, 1.0});
    GspGraphicsFillRect({size.width/2, 0.0, size.width/2, size.height}, {0.0, 1.0, 0.0, 1.0});
    //printf("WINDOW DRAW\n");
}


void ApplicationLaunched(std::vector<std::string> args) {
    printf("APPLICATION LAUNCHED CALLBACK\n");

    gWindow window;
    gCreateWindow(window, "Hello Gsp", { 800.0, 600.0 });
    gSetWindowVisibility(window, gWindowVisibility::VISIBLE);
    //gSetWindowResizeCallback(window, WindowResized);
    //gSetWindowMoveCallback(window, WindowMoved);
    gSetWindowDrawCallback(window, WindowDraw);
}


int main() {

    gApplicationInfo appInfo{};
    appInfo.quitWhenNoWindows = true;

    gApplicationSetLaunchedCallback(ApplicationLaunched);

    return gApplicationRun(appInfo);
}