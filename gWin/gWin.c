#include <gWin/gWin.h>
#include <gWin/x11/gWin_x11.h>

#include <stdio.h> // for printf
#include <stdlib.h> // for malloc etc

enum {
    NONE,
    WIN32,
    X11,
    WAYLAND,
} g_environment = 0;

static gwin_launch_callback_t g_launch_callback = 0;
static bool g_quit_on_window_close = true;

static bool g_should_quit_flag = false;

static size_t g_num_windows = 0;
static gwin_handle_t* g_window_handles = NULL;

// INTERNAL FUNCTIONS

void shrink_window_handles() {

    if (g_num_windows <= 1) {

        // array only has 1 element left so deallocate
        
        free(g_window_handles);
        g_window_handles = NULL;

        g_num_windows = 0;

    } else {

        // try to resize g_window_handles array
        gwin_handle_t* temp_array = (gwin_handle_t*)realloc(g_window_handles, (g_num_windows - 1) * sizeof(gwin_handle_t));

        if (temp_array == NULL) {
            printf("GWIN_ERROR: failed to resize window handle array\n");
        } else {
            g_window_handles = temp_array;
        }

        g_num_windows--;
    }
}

bool should_close_window(gwin_handle_t window) {


    return false;
}

void window_closed(gwin_handle_t window) {
    

    shrink_window_handles();

}

/**
 * @brief Sets the Global Launch Callback. 
 * Call before gwin_run
 * @param launch_callback The launch callback function.
 */
void gwin_set_launch_callback(gwin_launch_callback_t launch_callback) {
    g_launch_callback = launch_callback;
}

/**
 * @brief Configures gWin to quit automatically if there are no windows open.
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

    #ifdef __linux__
        if (!gwin_x11_init()) {
            return 1;
        } else {
            g_environment = X11;
        }
    #endif // LINUX

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

/**
 * @brief Creates an Application Window
 * @param handle A gwin handle to write the new window to.
 * @return 1 if the window was created successfully, otherwise 0.
 */
int gwin_create_window(gwin_handle_t* handle) {

    if (handle == NULL) {
        printf("GWIN_WARNING: invalid gwin_handle_t passed to gwin_create_window function\n");
        return 0;
    }

    printf("creating window\n");

    gwin_handle_t temp_handle = NULL;

    switch (g_environment) {
        case X11:
            if (!gwin_x11_create_window(&temp_handle)) {
                return 0;
            }
            break;
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
            switch (g_environment) {
                case X11:
                    gwin_x11_destroy_window(temp_handle);
                    break;
            }

            return 0;
        }

        // array successfully updated
        g_num_windows++;
        g_window_handles = temp_array; 

    } else {
        // try to resize g_window_handles array
        gwin_handle_t* temp_array = (gwin_handle_t*)realloc(g_window_handles, (g_num_windows + 1) * sizeof(gwin_handle_t));

        if (temp_array == NULL) {
            printf("GWIN_ERROR: failed to resize window handle array\n");

            // destroy window, ignore if failed
            switch (g_environment) {
                case X11:
                    gwin_x11_destroy_window(temp_handle);
                    break;
            }

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
 * @param handle A gwin handle to destroy
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
int gwin_destroy_window(gwin_handle_t handle) {

    if (handle == NULL) {
        printf("GWIN_WARNING: invalid gwin_handle_t passed to gwin_destroy_window function\n");
        return 0;
    }

    printf("destroying window\n");

    bool window_destroyed = false;

    switch (g_environment) {
        case X11:
            window_destroyed = gwin_x11_destroy_window(handle);
            break;
    }

    if (!window_destroyed) {
        return 0;
    }

    // should only get to here if window successfully destroyed

    shrink_window_handles();

    return 1;
}


