#include "GSPCore/GWindow.h"
#include "internal/def/GWindowDef.h"


#include "GSPCore/GLog.h"

#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

static Display* xDisplay = NULL;
static Window xRoot = None;
static XEvent xEvent;
static KeySym eventKey;
static char eventText[255];

static Atom deleteMessage;


GWindow GWindow_Init(GWindowInfo info) {

    if (xDisplay == NULL) {
        // either first time opening a window, or x11 broken.
        xDisplay = XOpenDisplay(NULL);
        xRoot = DefaultRootWindow(xDisplay);
    }

    if (xDisplay == NULL) {
        GLog(ERROR, "Failed to connect to an X11 display server!");
        return NULL;
    }

    if (xRoot == 0) {
        XCloseDisplay(xDisplay);
        GLog(ERROR, "Failed to connect to an X11 root window!");
        return NULL;
    }

    deleteMessage = XInternAtom(xDisplay, "WM_DELETE_WINDOW", False);

    Window temp_window = XCreateSimpleWindow(xDisplay, xRoot, 0, 0, info.width, info.height, 0, 0, 0xffffffff);
    if (temp_window == None) {
        GLog(ERROR, "Failed to create an X11 window");
        return 0;
    }

    XSetWMProtocols(xDisplay, temp_window, &deleteMessage, 1);
    XSelectInput(xDisplay, temp_window, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);
    XMapWindow(xDisplay, temp_window);

    GWindowDef* window = malloc(sizeof(GWindowDef));

    if (window == NULL) {
        return NULL;
    }

    window->rawHandle = (void*)temp_window;
    window->width = info.width;
    window->height = info.height;

    GLog(INFO, "Created an X window");

    return window;
}

// connect events after creations
void GWindow_SetResizeEvent(GWindow window, GWindowResizeEvent resizeEvent) {

}

void GWindowDef_Poll() {

    XNextEvent(xDisplay, &xEvent);

    switch(xEvent.type) {
        case Expose:
            // Handle window exposure (redraw window content)
            // For simplicity, let's just redraw a rectangle
            XFillRectangle(xDisplay, xEvent.xany.window, DefaultGC(xDisplay, 0), 20, 20, 100, 100);
            //if (g_window_paint_request_callback != NULL) {
            //    g_window_paint_request_callback((uintptr_t) g_event.xany.window);
            //}

            //if (g_window_paint_request_callback != NULL) {
            //    g_window_paint_request_callback((uintptr_t) g_event.xany.window);
           // }

            break;
        case ConfigureNotify:
            // Handle window resize
            // Adjust window size variables accordingly
            // For simplicity, let's just print the new size
            GLog(INFO, "Window resized: width=%d, height=%d", xEvent.xconfigure.width, xEvent.xconfigure.height);
            
            //if (g_window_resized_callback != NULL) {
            //    struct gwin_window_size_t size = {g_event.xconfigure.width, g_event.xconfigure.height};
            //    g_window_resized_callback((uintptr_t)g_event.xany.window, size);
            //}
            
            
            break;
        case KeyPress:
            // Handle keyboard events
            XLookupString(&xEvent.xkey, eventText, sizeof(eventText), &eventKey, 0);
            printf("Key pressed: %s\n", eventText);
            break;
        case ButtonPress:
            // Handle mouse button events
            printf("Mouse button pressed\n");
            break;
        case ClientMessage:
            /*if (g_event.xclient.data.l[0] == g_delete_message) {
                if (g_window_close_request_callback != NULL) {

                    if (g_window_close_request_callback((uintptr_t) g_event.xany.window)) {

                        gwin_destroy_window((uintptr_t) g_event.xany.window);
                    }
                } else {
                    gwin_destroy_window((uintptr_t) g_event.xany.window);
                }
            }*/
            break;
    }
}