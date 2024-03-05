#include <stdlib.h>
#include <stdio.h>

// X11 headers
#include <X11/Xlib.h>

// Wayland headers
#include <wayland-client.h>

int main() {

    
    wl_display* waylandDisplay = nullptr;
    Display* xDisplay = nullptr;

    // try connecting to wayland first
    waylandDisplay = wl_display_connect(nullptr);

    if (waylandDisplay == nullptr) {
        xDisplay = XOpenDisplay(nullptr);
    }

    if (waylandDisplay != nullptr) {
        printf("Running on Wayland\n");
    } else if (xDisplay != nullptr) {
        printf("Running on X11\n");
    } else {
        printf("No display server found!\n");
        return -1;
    }
    

    return 0;
}