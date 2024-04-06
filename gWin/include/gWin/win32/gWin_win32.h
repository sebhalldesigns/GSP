/**
 * @file gWin_win32.h
 * @brief This file contains the interface for the gWin win32-specific implmentation.
 */

#ifndef GWIN_WIN32_H
#define GWIN_WIN32_H

#include <gWin/gWin.h>

#include <stddef.h> // for size_t

/**
 * @brief Tries to initialize gWin for win32.
 * @return 1 if the initialization succeeds, 0 if it fails.
 */
int gwin_win32_init();

/**
 * @brief Finalizes gWin for win32.
 */
void gwin_win32_final();


/**
 * @brief Polls win32 events.
 */
void gwin_win32_poll_events();

/**
 * @brief Creates an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to write the new window to.
 * @return 1 if the window was created successfully, otherwise 0.
 */
int gwin_win32_create_window(gwin_handle_t* handle, gwin_window_creation_options_t options);

/**
 * @brief Destroys an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to destroy. This should have been checked already by gWin.c
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
int gwin_win32_destroy_window(gwin_handle_t handle);



#endif // GWIN_WIN32_H