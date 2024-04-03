/**
 * @file gWin_macOS.h
 * @brief This file contains the interface for the gWin macOS-specific implmentation.
 */

#ifndef GWIN_MACOS_H
#define GWIN_MACOS_H

#include <gWin/gWin.h>
#include <Cocoa/Cocoa.h>
#include <MetalKit/MetalKit.h>

#include <stddef.h> // for size_t

/**
 * @brief Tries to initialize gWin for macOS.
 * @return 1 if the initialization succeeds, 0 if it fails.
 */
int gwin_macOS_init();

/**
 * @brief Finalizes gWin for macOS.
 */
void gwin_macOS_final();


/**
 * @brief Polls macOS events.
 */
void gwin_macOS_poll_events();

/**
 * @brief Creates an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to write the new window to.
 * @return 1 if the window was created successfully, otherwise 0.
 */
int gwin_macOS_create_window(gwin_handle_t* handle);

/**
 * @brief Destroys an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to destroy. This should have been checked already by gWin.c
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
int gwin_macOS_destroy_window(gwin_handle_t handle);

@interface AppDelegate : NSObject <NSApplicationDelegate> @end
@interface WindowDelegate : NSObject <NSWindowDelegate, MTKViewDelegate> @end

#endif // GWIN_MACOS_H
