/**
 * @file gWin.h
 * @brief This file contains the gWin module interface. It should be included when using gWin, and other header files in gWin ignored.
 * You should set a launch callback and call gwin_run in the main C function.
 * Windows can then be created and interacted with from the launch callback.
 */

#ifndef GWIN_H
#define GWIN_H

#include <gVG/gVG.h>

#include <stdbool.h> // for bool type
#include <stdint.h> // for uintptr_t

// TYPE DEFINITIONS

/**
 * @typedef Window Handle type
 * @brief An abstraction over the raw window handle. 
 * Clients should not try and interact with the window handle directly,
 * but should instead use it as a reference that is passed to gWin.
 */
typedef uintptr_t gwin_handle_t;

/**
 * @typedef Window Size type
 * @brief represents a width and height in coordinate space
 */
typedef struct {
    int width;
    int height;
} gwin_window_size_t;

typedef enum {
    SOFTWARE_BUFFER,
    HARDWARE_BUFFER
} gwin_window_buffer_type;

typedef struct {
    gwin_window_size_t size;
    char* title;
    bool start_hidden;
    bool center_screen;
} gwin_window_creation_options_t;

/**
 * @typedef gWin Launch Callback type
 * @brief Call gwin_set_launch_callback before gwin_run to set the global launch callback.
 */
typedef void (*gwin_launch_callback_t)();

/**
 * @typedef gWin Window Resized Callback type
 * @brief Call gwin_set_window_resized_callback before gwin_run to set the global window resize callback.
 * Use this callback to respond to window resizing.
 * @param window The window that was resized.
 * @param size The new window size.
 */
typedef void (*gwin_window_resized_callback_t)(gwin_handle_t window, gwin_window_size_t size);

/**
 * @typedef gWin Window Resize Request Callback type
 * @brief Call gwin_set_window_resize_request_callback before gwin_run to set the global window resize request callback.
 * Use this callback if you wish to interfere with window resizing (such as setting a limits on the window size, or 'snapping' the window size to a particular value).
 * @param window The window that is being resized.
 * @param requested_size The requested size.
 * @return The adjusted size for the window. This can be the same or different to the requested size.
 */
typedef struct gwin_window_size_t (*gwin_window_resize_request_callback_t)(gwin_handle_t window, gwin_window_size_t requested_size);

/**
 * @typedef gWin Window Closed Callback type
 * @brief Call gwin_set_window_closed_callback before gwin_run to set the global launch callback.
 * Use this callback to destroy resources that were associated with the closed window.
 * @param window The window that has been closed.
 */
typedef void (*gwin_window_closed_callback_t)(gwin_handle_t window);

/**
 * @typedef gWin Window Close Request Callback type
 * @brief Call gwin_set_window_close_request_callback before gwin_run to set the global launch callback.
 * Use this callback to prevent a window from being closed when the user presses the 'close' button. This might be to prompt the user to save their work, for example.
 * @param window The window that the close request is for.
 * @return true if the window should close, false if it should remain open.
 */
typedef bool (*gwin_window_close_request_callback_t)(gwin_handle_t window);

/**
 * @typedef gWin Window Paint Request Callback type
 * @brief Call gwin_set_window_paint_request_callback before gwin_run to set the global launch callback.
 * Use paint commands in this callback. If this callback is not set, all windows will be a solid white color.
 * @param window The window that the paint request is for.
 */
typedef void (*gwin_window_paint_request_callback_t)(gwin_handle_t window);


// GLOBAL GWIN FUNCTIONS

/**
 * @brief Configures gWin to quit automatically if there are no windows open. 
 * The default value is true.
 * @param quit_on_window_close Pass true if gWin should quit afte the last window is closed
 * and false if it shouldn't.
 */
void gwin_set_quit_on_window_close(bool quit_on_window_close);

/**
 * @brief Runs the Main Application Event Loop.
 * @return 0 if the application exited successfully, 1 if there was an error.
 */
int gwin_run();

// CALLBACK SETTINGS

/**
 * @brief Sets the Global Launch Callback. 
 * Call before gwin_run
 * @param launch_callback The launch callback function.
 */
void gwin_set_launch_callback(gwin_launch_callback_t launch_callback);

/**
 * @brief Sets the Global Window Resized Callback. 
 * Call before gwin_run
 * @param window_resized_callback The window resized callback function.
 */
void gwin_set_window_resized_callback(gwin_window_resized_callback_t window_resized_callback);

/**
 * @brief Sets the Global Window Resize Request Callback. 
 * Call before gwin_run
 * @param window_resize_request The window resize request callback function.
 */
void gwin_set_window_resize_request_callback(gwin_window_resize_request_callback_t window_resize_request_callback);

/**
 * @brief Sets the Global Window Closed Callback. 
 * Call before gwin_run
 * @param window_closed_callback The window closed callback function.
 */
void gwin_set_window_closed_callback(gwin_window_closed_callback_t window_closed_callback);

/**
 * @brief Sets the Global Window Close Request Callback. 
 * Call before gwin_run
 * @param window_close_request The window resize request callback function.
 */
void gwin_set_window_close_request_callback(gwin_window_close_request_callback_t window_close_request_callback);

/**
 * @brief Sets the Global Window Paint Request Callback. 
 * Call before gwin_run
 * @param window_paint_request The window paint request callback function.
 */
void gwin_set_window_paint_request_callback(gwin_window_close_request_callback_t window_paint_request_callback);


// WINDOW LEVEL FUNCTIONS

/**
 * @brief Creates an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to write the new window to.
 * @return 1 if the window was created successfully, otherwise 0.
 */
int gwin_create_window(gwin_handle_t* handle, gwin_window_creation_options_t options);

/**
 * @brief Destroys an Application Window
 * @param handle A gWin handle of the window.
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
int gwin_destroy_window(gwin_handle_t handle);

/**
 * @brief Paints a gVG buffer to a window.
 * @param handle The gWin handle of the window.
 * @param buffer The gVG buffer to paint.
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
void gwin_paint_gvg(gwin_handle_t handle, struct gvg_buffer_t buffer);




#endif // GWIN_H
