#include "GSPCore/GWindow.h"
#include "internal/def/GWindowDef.h"
#include "internal/include/GVector.h"



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

GVector windowVector = NULL;

// functions defined in this file
GWindowDef* TryGetWindow(Window xWindow);

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

    Window xWindow = XCreateSimpleWindow(xDisplay, xRoot, 0, 0, info.width, info.height, 0, 0, 0xffffffff);
    if (xWindow == None) {
        GLog(ERROR, "Failed to create an X11 window");
        return 0;
    }

    XSetWMProtocols(xDisplay, xWindow, &deleteMessage, 1);
    XSelectInput(xDisplay, xWindow, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
    XMapWindow(xDisplay, xWindow);

    GWindowDef* window = malloc(sizeof(GWindowDef));

    if (window == NULL) {
        return NULL;
    }

    window->rawHandle = (void*)xWindow;
    window->width = info.width;
    window->height = info.height;


    if (windowVector == NULL) {
        windowVector = GVector_Init();
    }

    if (windowVector != NULL) {
        GVector_Add(windowVector, (GVectorItem)window);
        GLog(INFO, "Created an X window");
        return window;
    } else {
        GLog(ERROR, "Failed to allocate a window vector.");
        return NULL;
    }

}

void GWindow_SetResizeDelegate(GWindow window, GWindowResizeDelegate resizeDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->resizeDelegate = resizeDelegate;
    }
}

void GWindow_SetPointerMoveDelegate(GWindow window, GWindowPointerMoveDelegate pointerMoveDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->pointerMoveDelegate = pointerMoveDelegate;
    }
}

void GWindow_SetButtonDownDelegate(GWindow window, GWindowButtonDownDelegate buttonDownDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->buttonDownDelegate = buttonDownDelegate;
    }
}

void GWindow_SetButtonUpDelegate(GWindow window, GWindowButtonUpDelegate buttonUpDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->buttonUpDelegate = buttonUpDelegate;
    }
}



void GWindowDef_Poll() {

    XNextEvent(xDisplay, &xEvent);

    #ifdef DEBUG 
    {
        GLog(INFO, "ConfigureNotify event");
    } 
    #endif

    GWindowDef* windowDef = TryGetWindow(xEvent.xany.window);

    if (windowDef == NULL) {
        #ifdef DEBUG 
        {
            GLog(ERROR, "Event recieved for unknown window!");
        } 
        #endif
        return;
    }

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
            
            int newWidth = xEvent.xconfigure.width;
            int newHeight = xEvent.xconfigure.height;

            if (windowDef->width != newWidth || windowDef->height != newHeight) {

                GWindowSize newSize = { newWidth, newHeight };

                windowDef->width = newWidth;
                windowDef->height = newHeight;
                
                if (windowDef->resizeDelegate != NULL) {
                    (windowDef->resizeDelegate)(windowDef, newSize);
                }
            }
            
            break;
        case MotionNotify:

            GWindowPoint motionLocation = {xEvent.xmotion.x, xEvent.xmotion.y};
            if (windowDef->pointerMoveDelegate != NULL) {
                (windowDef->pointerMoveDelegate)(windowDef, motionLocation);
            }

            break;
        case ButtonPress:
        
            GWindowPoint buttonDownLocation = {xEvent.xbutton.x, xEvent.xbutton.y};
            if (windowDef->buttonDownDelegate != NULL) {
                (windowDef->buttonDownDelegate)(windowDef, buttonDownLocation, (uint8_t)xEvent.xbutton.button);
            }

            break;
        case ButtonRelease:

            GWindowPoint buttonUpLocation = {xEvent.xbutton.x, xEvent.xbutton.y};
            if (windowDef->buttonUpDelegate != NULL) {
                (windowDef->buttonUpDelegate)(windowDef, buttonUpLocation, (uint8_t)xEvent.xbutton.button);
            }

            break;
        case KeyPress:
            // Handle keyboard events
            XLookupString(&xEvent.xkey, eventText, sizeof(eventText), &eventKey, 0);
            printf("Key pressed: %s\n", eventText);
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

GWindowDef* TryGetWindow(Window xWindow) {
    for (int i = 0; i < GVector_Size(windowVector); i++) {
        if (((GWindowDef*)GVector_Get(windowVector, i))->rawHandle == (GVectorItem)xEvent.xany.window) {
            return (GWindowDef*)GVector_Get(windowVector, i);
        }
    }
        
    return NULL;
}