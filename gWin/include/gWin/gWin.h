/**
 * @file gWin.h
 * @brief This file contains the gWin module interface. It should be included when using gWin, and other header files in gWin ignored.
 * You should set a launch callback and call gwin_run in the main C function.
 * Windows can then be created and interacted with from the launch callback.
 */

#ifndef GWIN_H
#define GWIN_H

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
 * @typedef gWin Launch Callback type
 * @brief Call gwin_set_launch_callback before gwin_run to set the global launch callback.
 */
typedef void (*gwin_launch_callback_t)();



// GLOBAL GWIN FUNCTIONS

/**
 * @brief Sets the Global Launch Callback. 
 * Call before gwin_run
 * @param launch_callback The launch callback function.
 */
void gwin_set_launch_callback(gwin_launch_callback_t launch_callback);

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





// WINDOW LEVEL FUNCTIONS

/**
 * @brief Creates an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to write the new window to.
 * @return 1 if the window was created successfully, otherwise 0.
 */
int gwin_create_window(gwin_handle_t* handle);

/**
 * @brief Destroys an Application Window
 * @param handle A gWin handle of the window.
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
int gwin_destroy_window(gwin_handle_t handle);







#endif // GWIN_H
