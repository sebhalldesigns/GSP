#include <gWin/gWin.h>
#include <gWin/x11/gWin_x11.h>

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

static gwin_x11_window_close_request_callback_t g_window_close_request_callback = NULL;
static gwin_x11_window_closed_callback_t g_window_closed_callback = NULL;

static Display* g_x11_display = NULL;
static Window g_x11_root = None;
static XEvent g_event;
static KeySym g_event_key;
static char g_event_text[255];


static Atom g_delete_message;

/**
 * @brief Tries to initialize gWin for x11.
 * @return 1 if the initialization succeeds, 0 if it fails.
 */
int gwin_x11_init() {
    
    printf("GWIN_INFO: trying to initialize for X11\n");

    g_x11_display = XOpenDisplay(NULL);

    if (g_x11_display == NULL) {
        printf("GWIN_ERROR: failed to find an X display server.\n");
        return 0;
    }

    g_x11_root = DefaultRootWindow(g_x11_display);

    if (g_x11_root == None) {
        XCloseDisplay(g_x11_display);
        printf("GWIN_ERROR: failed to find a root X window.\n");
        return 0;
    }

    printf("GWIN_INFO: X11 initialization succeeeded\n");

    g_delete_message = XInternAtom(g_x11_display, "WM_DELETE_WINDOW", False);

    return 1;
}

/**
 * @brief Sets the Internal Close Request Callback
 * Called by gWin system.
 * @param window_close_request_callback The close request callback function.
 */
void gwin_x11_set_window_close_request_callback(gwin_x11_window_close_request_callback_t window_close_request_callback) {
    g_window_close_request_callback = window_close_request_callback;
}

/**
 * @brief Sets the Internal Closed Callback
 * Called by gWin system.
 * @param window_closed_callback The closed callback .
 */
void gwin_x11_set_window_closed_callback(gwin_x11_window_closed_callback_t window_closed_callback) {

}


/**
 * @brief Finalizes gWin for x11.
 */
void gwin_x11_final() {
    XCloseDisplay(g_x11_display);
}

/**
 * @brief Polls X11 events.
 */
void gwin_x11_poll_events() {

    XNextEvent(g_x11_display, &g_event);

    switch(g_event.type) {
        case Expose:
            // Handle window exposure (redraw window content)
            // For simplicity, let's just redraw a rectangle
            XFillRectangle(g_x11_display, g_event.xany.window, DefaultGC(g_x11_display, 0), 20, 20, 100, 100);
            break;
        case ConfigureNotify:
            // Handle window resize
            // Adjust window size variables accordingly
            // For simplicity, let's just print the new size
            printf("Window resized: width=%d, height=%d\n", g_event.xconfigure.width, g_event.xconfigure.height);
            break;
        case KeyPress:
            // Handle keyboard events
            XLookupString(&g_event.xkey, g_event_text, sizeof(g_event_text), &g_event_key, 0);
            printf("Key pressed: %s\n", g_event_text);
            break;
        case ButtonPress:
            // Handle mouse button events
            printf("Mouse button pressed\n");
            break;
        case ClientMessage:
            if (g_event.xclient.data.l[0] == g_delete_message) {
                if (g_window_close_request_callback != NULL) {

                    if (g_window_close_request_callback((uintptr_t) g_event.xany.window)) {

                        XDestroyWindow(g_x11_display, g_event.xany.window);
                        printf("GWIN_INFO: X11 window destroyed\n");

                        if (g_window_closed_callback  != NULL) {
                            g_window_closed_callback(g_event.xany.window);
                        }
                    }
                } else {
                    XDestroyWindow(g_x11_display, g_event.xany.window);
                    printf("GWIN_INFO: X11 window destroyed\n");

                    if (g_window_closed_callback  != NULL) {
                        g_window_closed_callback(g_event.xany.window);
                    }
                }
            }
            break;
    }
}

/**
 * @brief Creates an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to write the new window to.
 * @return 1 if the window was created successfully, otherwise 0.
 */
int gwin_x11_create_window(gwin_handle_t* handle) {
    /*if (g_x11_display == NULL) {

    }*/

    Window temp_window = XCreateSimpleWindow(g_x11_display, g_x11_root, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0, 0, 0xffffffff);
    if (temp_window == None) {
        printf("GWIN_ERROR: failed to create an X window.\n");
        return 0;
    }

    XSetWMProtocols(g_x11_display, temp_window, &g_delete_message, 1);

    XSelectInput(g_x11_display, temp_window, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);

    XMapWindow(g_x11_display, temp_window);

    *handle = (uintptr_t)temp_window;

    printf("GWIN_INFO: created an X window\n");

    return 1; 
}

/**
 * @brief Destroys an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to destroy. This should have been checked already by gWin.c
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
int gwin_x11_destroy_window(gwin_handle_t handle) {
    return 1;
}