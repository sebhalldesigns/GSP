#include <gWin/gWin.h>
#include <gWin/x11/gWin_x11.h>

#include <stdlib.h>
#include <stddef.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

// STANDARD GLOBALS
static gwin_launch_callback_t g_launch_callback = NULL;
static gwin_window_resized_callback_t g_window_resized_callback = NULL;
static gwin_window_resize_request_callback_t g_window_resize_request_callback = NULL;
static gwin_window_closed_callback_t g_window_closed_callback = NULL;
static gwin_window_close_request_callback_t g_window_close_request_callback = NULL;
static gwin_window_paint_request_callback_t g_window_paint_request_callback = NULL;

static bool g_quit_on_window_close = true;
static bool g_should_quit_flag = false;

static size_t g_num_windows = 0;
static gwin_handle_t* g_window_handles = NULL;


// X11 SPECIFIC GLOBALS
static Display* g_x11_display = NULL;
static Window g_x11_root = None;
static XEvent g_event;
static KeySym g_event_key;
static char g_event_text[255];

static Atom g_delete_message;

// INTERNAL GWIN HELPER FUNCTIONS

// safely remove a window from the buffer if it exists
void remove_window_from_buffer(gwin_handle_t window) {

    if (g_num_windows == 1 && g_window_handles[0] == window) {
        printf("GWIN_INFO: removing last window from buffer...\n");
        free(g_window_handles);
        g_window_handles = NULL;
        g_num_windows = 0;
        return;
    }

    size_t num_removed = 0;

    size_t i = 0;
    while (i < g_num_windows) {
        printf("window at buffer is %llu and to remove is %llu\n", g_window_handles[i], window);
        if (g_window_handles[i] == window) {
            // found an instance of the window
            printf("GWIN_INFO: found a window to remove from the buffer\n");
            
            size_t j = 0;
            while (j < g_num_windows - 1) {
                g_window_handles[j] = g_window_handles[j + 1];
                j++;
            }

            // try to resize g_window_handles array
            gwin_handle_t* temp_array = (gwin_handle_t*)realloc(g_window_handles, (g_num_windows - 1) * sizeof(gwin_handle_t));

            if (temp_array == NULL) {
                printf("GWIN_ERROR: failed to resize window handle array\n");
            } else {
                g_window_handles = temp_array;
            }

            g_num_windows--;

        }

        i++;
    }
}

// GLOBAL GWIN FUNCTIONS

/**
 * @brief Configures gWin to quit automatically if there are no windows open. 
 * The default value is true.
 * @param quit_on_window_close Pass true if gWin should quit afte the last window is closed
 * and false if it shouldn't.
 */
void gwin_set_quit_on_window_close(bool quit_on_window_close) {
    g_quit_on_window_close = quit_on_window_close;
}

/**
 * @brief Runs the Main Application Event Loop.
 * @return 0 if the application exited successfully, 1 if there was an error.
 */
int gwin_run() {

    if (!gwin_x11_init()) {
        return 1;
    }

    if (g_launch_callback != NULL) {
        g_launch_callback();
    }

    while (!g_should_quit_flag) {
        gwin_x11_poll_events();

        if (g_quit_on_window_close && g_num_windows == 0) {
            printf("GWIN_INFO: quitting due quit_on_window_close being set to true and no windows being open\n");
            g_should_quit_flag = true;
        }
    }


    return 0;
}

// CALLBACK SETTINGS

/**
 * @brief Sets the Global Launch Callback. 
 * Call before gwin_run
 * @param launch_callback The launch callback function.
 */
void gwin_set_launch_callback(gwin_launch_callback_t launch_callback) {
    g_launch_callback = launch_callback;
}

/**
 * @brief Sets the Global Window Resized Callback. 
 * Call before gwin_run
 * @param window_resized_callback The window resized callback function.
 */
void gwin_set_window_resized_callback(gwin_window_resized_callback_t window_resized_callback) {
    g_window_resized_callback = window_resized_callback;
}

/**
 * @brief Sets the Global Window Resize Request Callback. 
 * Call before gwin_run
 * @param window_resize_request The window resize request callback function.
 */
void gwin_set_window_resize_request_callback(gwin_window_resize_request_callback_t window_resize_request_callback) {
    g_window_resize_request_callback = window_resize_request_callback;
}

/**
 * @brief Sets the Global Window Closed Callback. 
 * Call before gwin_run
 * @param window_closed_callback The window closed callback function.
 */
void gwin_set_window_closed_callback(gwin_window_closed_callback_t window_closed_callback) {
    g_window_closed_callback = window_closed_callback;
}

/**
 * @brief Sets the Global Window Close Request Callback. 
 * Call before gwin_run
 * @param window_close_request The window resize request callback function.
 */
void gwin_set_window_close_request_callback(gwin_window_close_request_callback_t window_close_request_callback) {
    g_window_close_request_callback = window_close_request_callback;
}

/**
 * @brief Sets the Global Window Paint Request Callback. 
 * Call before gwin_run
 * @param window_paint_request The window paint request callback function.
 */
void gwin_set_window_paint_request_callback(gwin_window_close_request_callback_t window_paint_request_callback) {
    g_window_paint_request_callback = window_paint_request_callback;
}


// WINDOW LEVEL FUNCTIONS

/**
 * @brief Creates an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to write the new window to.
 * @return 1 if the window was created successfully, otherwise 0.
 */
int gwin_create_window(gwin_handle_t* handle) {

    if (handle == NULL) {
        printf("GWIN_WARNING: invalid gwin_handle_t passed to gwin_create_window function\n");
        return 0;
    }

    printf("creating window\n");

    gwin_handle_t temp_handle = NULL;

    if (!gwin_x11_create_window(&temp_handle)) {
        return 0;
    }
            
    if (temp_handle == NULL) {
        // creating window failed
        return 0;
    }

    if (g_num_windows == 0) {

        // array not allocated so allocate it
        gwin_handle_t* temp_array = (gwin_handle_t*)malloc(sizeof(gwin_handle_t));

        if (temp_array == NULL) {
            printf("GWIN_ERROR: failed to allocate window handles array\n");

            // destroy window, ignore if failed
            gwin_x11_destroy_window(temp_handle);

            return 0;
        }

        // array successfully updated
        g_window_handles = temp_array; 
        g_window_handles[g_num_windows] = temp_handle;
        g_num_windows++;

    } else {
        // try to resize g_window_handles array
        gwin_handle_t* temp_array = (gwin_handle_t*)realloc(g_window_handles, (g_num_windows + 1) * sizeof(gwin_handle_t));

        if (temp_array == NULL) {
            printf("GWIN_ERROR: failed to resize window handle array\n");

            // destroy window, ignore if failed
            gwin_x11_destroy_window(temp_handle);
                    
            return 0;
        }

        // array successfully updated
        g_num_windows++;
        g_window_handles = temp_array; 
    }

    *handle = temp_handle; 
    
    return 1;
}

/**
 * @brief Destroys an Application Window
 * @param handle A gWin handle of the window.
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
int gwin_destroy_window(gwin_handle_t handle) {
    printf("destroying window...\n");

    gwin_x11_destroy_window(handle);

    printf("removing window...\n");
    remove_window_from_buffer(handle);
        

    return 0;
}

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
            //XFillRectangle(g_x11_display, g_event.xany.window, DefaultGC(g_x11_display, 0), 20, 20, 100, 100);
            //if (g_window_paint_request_callback != NULL) {
            //    g_window_paint_request_callback((uintptr_t) g_event.xany.window);
            //}

            if (g_window_paint_request_callback != NULL) {
                g_window_paint_request_callback((uintptr_t) g_event.xany.window);
            }

            break;
        case ConfigureNotify:
            // Handle window resize
            // Adjust window size variables accordingly
            // For simplicity, let's just print the new size
            //printf("Window resized: width=%d, height=%d\n", g_event.xconfigure.width, g_event.xconfigure.height);
            
            if (g_window_resized_callback != NULL) {
                struct gwin_window_size_t size = {g_event.xconfigure.width, g_event.xconfigure.height};
                g_window_resized_callback((uintptr_t)g_event.xany.window, size);
            }
            
            
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

                        gwin_destroy_window((uintptr_t) g_event.xany.window);
                    }
                } else {
                    gwin_destroy_window((uintptr_t) g_event.xany.window);
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

    printf("GWIN_INFO: created an X window %llu\n", (uintptr_t)temp_window);

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

    XDestroyWindow(g_x11_display, handle);
    printf("GWIN_INFO: X11 window destroyed %llu\n", handle);

    if (g_window_closed_callback != NULL) {
        g_window_closed_callback(g_event.xany.window);
    }

    return 1;
}

/**
 * @brief Paints a gVG buffer to a window.
 * @param handle The gWin handle of the window.
 * @param buffer The gVG buffer to paint.
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
void gwin_paint_gvg(gwin_handle_t handle, struct gvg_buffer_t buffer) {

    XImage* ximage = XCreateImage(g_x11_display, DefaultVisual(g_x11_display, 0), 24, ZPixmap, 0,
                                   (char *)buffer.data, buffer.width, buffer.height, 32, 0);

    XPutImage(g_x11_display, handle, DefaultGC(g_x11_display, 0), ximage, 0, 0, 0, 0, buffer.width, buffer.height);
    XFlush(g_x11_display);

    //XDestroyImage(ximage);
}