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
    XSelectInput(xDisplay, xWindow, ExposureMask | KeyPressMask | ButtonPressMask | ResizeRedirectMask | StructureNotifyMask);
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

void GWindow_SetWillResizeDelegate(GWindow window, GWindowWillResizeDelegate willResizeDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->willResizeDelegate = willResizeDelegate;
    }
}

void GWindowDef_Poll() {

    #ifdef DEBUG 
    {
        //GLog(INFO, "Polling for X events");
    } 
    #endif

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
        case ResizeRequest:

        /*XWindowAttributes attr;
  XGetWindowAttributes(display, window, &attr);
  if (attr.width != desired_width || attr.height != desired_height) {
    XResizeWindow(display, window, desired_width, desired_height);
  }*/

            // not working properly
            //return;

            #ifdef DEBUG 
            {
                //GLog(INFO, "ConfigureNotify event");
            } 
            #endif

            GWindowDef* resizeWindowDef = NULL;

            for (int i = 0; i < GVector_Size(windowVector); i++) {
                if (((GWindowDef*)GVector_Get(windowVector, i))->rawHandle == (GVectorItem)xEvent.xany.window) {
                    resizeWindowDef = (GWindowDef*)GVector_Get(windowVector, i);
                }
            }

            if (resizeWindowDef == NULL) {
                #ifdef DEBUG 
                {
                    GLog(ERROR, "Event recieved for unknown window!");
                } 
                #endif
                    
                return;
            }

            int width = xEvent.xresizerequest.width;
            int height = xEvent.xresizerequest.height;

            printf("new size request: %d %d\n", width, height);

            GWindowSize newSize = { width, height };

            if (resizeWindowDef->willResizeDelegate != NULL) {

                GWindowSize adjustedSize = (resizeWindowDef->willResizeDelegate)(resizeWindowDef, newSize);
                if (newSize.width != adjustedSize.width || newSize.height != adjustedSize.height) {
                    
                    #ifdef DEBUG 
                    {
                        GLog(INFO, "New window size overridden to %d %d", adjustedSize.width, adjustedSize.height);
                    } 
                    #endif

                    resizeWindowDef->width = adjustedSize.width;
                    resizeWindowDef->height = adjustedSize.height;

                    // i.e window size was overriden
                    XResizeWindow(xDisplay, xEvent.xany.window, adjustedSize.width, adjustedSize.height);
 
                    return;
                }
            }

            XConfigureWindow(xDisplay, xEvent.xany.window, CWWidth | CWHeight, &(XWindowChanges){.width = resizeWindowDef->width, .height = resizeWindowDef->height});

            break;
        case ConfigureNotify:

            #ifdef DEBUG 
            {
                //GLog(INFO, "ConfigureNotify event");
            } 
            #endif

            GWindowDef* windowDef = NULL;

            for (int i = 0; i < GVector_Size(windowVector); i++) {
                if (((GWindowDef*)GVector_Get(windowVector, i))->rawHandle == (GVectorItem)xEvent.xany.window) {
                    windowDef = (GWindowDef*)GVector_Get(windowVector, i);
                }
            }

            if (windowDef == NULL) {
                #ifdef DEBUG 
                {
                    GLog(ERROR, "Event recieved for unknown window!");
                } 
                #endif
                    
                return;
            }
            
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