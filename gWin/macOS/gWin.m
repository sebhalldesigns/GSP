#include <gWin/gWin.h>
#include <gWin/macOS/gWin_macOS.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#import <Cocoa/Cocoa.h>

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

// MACOS GLOBALS
NSApplication* application = NULL;
AppDelegate* appDelegate = NULL;
WindowDelegate* windowDelegate = NULL;

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

    if (!gwin_macOS_init()) {
        return 1;
    }

    application = [NSApplication sharedApplication];
    appDelegate = [[AppDelegate alloc] init];
    windowDelegate = [[WindowDelegate alloc] init];
    [application setDelegate:appDelegate];
    [application run];

    /*
    if (g_launch_callback != NULL) {
        g_launch_callback();
    }

    while (!g_should_quit_flag) {
        printf("POLLING...");
        gwin_macOS_poll_events();

        if (g_quit_on_window_close && g_num_windows == 0) {
            printf("GWIN_INFO: quitting due quit_on_window_close being set to true and no windows being open\n");
            g_should_quit_flag = true;
        }
    }*/


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

    printf("Creating window...\n");

	NSUInteger styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;    NSRect frame = NSMakeRect(0, 0, 600, 400);
    NSWindow* window = [[NSWindow alloc] initWithContentRect:frame styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];
    [window setDelegate: windowDelegate];
    const char* windowTitle = "GSP Window";
	[window setTitle: [NSString stringWithUTF8String: windowTitle]];
	[window makeKeyAndOrderFront: nil];

    return 1;
}  

/**
 * @brief Destroys an Application Window
 * @param handle A gWin handle of the window.
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
int gwin_destroy_window(gwin_handle_t handle) {
    return 1;
}

/**
 * @brief Paints a gVG buffer to a window.
 * @param handle The gWin handle of the window.
 * @param buffer The gVG buffer to paint.
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
void gwin_paint_gvg(gwin_handle_t handle, struct gvg_buffer_t buffer) {

}

/**
 * @brief Tries to initialize gWin for macOS.
 * @return 1 if the initialization succeeds, 0 if it fails.
 */
int gwin_macOS_init() {
    return 1;
}

/**
 * @brief Finalizes gWin for macOS.
 */
void gwin_macOS_final() {

}


/**
 * @brief Polls macOS events.
 */
void gwin_macOS_poll_events() {

}

/**
 * @brief Creates an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to write the new window to.
 * @return 1 if the window was created successfully, otherwise 0.
 */
int gwin_macOS_create_window(gwin_handle_t* handle) {
    return 1;
}

/**
 * @brief Destroys an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to destroy. This should have been checked already by gWin.c
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
int gwin_macOS_destroy_window(gwin_handle_t handle) {
    return 1;
}
    

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    printf("Application did finish launching.\n");
    if (g_launch_callback != NULL) {
        g_launch_callback();
    }
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    printf("Application will terminate.\n");
}

@end

@implementation WindowDelegate



- (void)mtkView:(MTKView * )view drawableSizeWillChange:(CGSize)size {
	
}

- (void)drawInMTKView:(MTKView * )view {
	
	
}

@end

