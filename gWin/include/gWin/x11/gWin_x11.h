/**
 * @file gWin_x11.h
 * @brief This file contains the interface for the gWin X11-specific implmentation.
 */

#ifndef GWIN_X11_H
#define GWIN_X11_H

#include <gWin/gWin.h>

#include <stddef.h> // for size_t

/**
 * @typedef gWin Internal Close Request Callback (X11)
 * @brief set by system.
 * @param window The window this callback is referring to.
 * @return true if the window should be closed, false if it should stay open. Defaults to true.
 */
typedef bool (*gwin_x11_window_close_request_callback_t)(gwin_handle_t window);

/**
 * @typedef gWin Internal Window Closed Callback (X11)
 * @brief set by system.
 * @param window The window this callback is referring to.
 */
typedef void (*gwin_x11_window_closed_callback_t)(gwin_handle_t window);

/**
 * @brief Tries to initialize gWin for x11.
 * @return 1 if the initialization succeeds, 0 if it fails.
 */
int gwin_x11_init();

/**
 * @brief Sets the Internal Close Request Callback
 * Called by gWin system.
 * @param window_close_request_callback The close request callback function.
 */
void gwin_x11_set_window_close_request_callback(gwin_x11_window_close_request_callback_t window_close_request_callback);

/**
 * @brief Sets the Internal Closed Callback
 * Called by gWin system.
 * @param window_closed_callback The closed callback .
 */
void gwin_x11_set_window_closed_callback(gwin_x11_window_closed_callback_t window_closed_callback);


/**
 * @brief Finalizes gWin for x11.
 */
void gwin_x11_final();


/**
 * @brief Polls X11 events.
 */
void gwin_x11_poll_events();

/**
 * @brief Creates an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to write the new window to.
 * @return 1 if the window was created successfully, otherwise 0.
 */
int gwin_x11_create_window(gwin_handle_t* handle);

/**
 * @brief Destroys an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to destroy. This should have been checked already by gWin.c
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
int gwin_x11_destroy_window(gwin_handle_t handle);



#endif // GWIN_X11_H