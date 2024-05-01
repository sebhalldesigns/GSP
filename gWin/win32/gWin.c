#include <gWin/gWin.h>
#include <gWin/win32/gWin_win32.h>

#include <stdlib.h>
#include <stddef.h>

#ifndef UNICODE
#define UNICODE
#endif 
#include <Windows.h>
#include <wchar.h>

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


// WIN32 SPECIFIC GLOBALS
static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
const wchar_t CLASS_NAME[]  = L"GSP Window";
static MSG g_msg;


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

    if (!gwin_win32_init()) {
        return 1;
    }

    if (g_launch_callback != NULL) {
        g_launch_callback();
    }

    while (!g_should_quit_flag) {
        printf("POLLING...");
        gwin_win32_poll_events();

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
int gwin_create_window(gwin_handle_t* handle, gwin_window_creation_options_t options) {

    if (handle == NULL) {
        printf("GWIN_WARNING: invalid gwin_handle_t passed to gwin_create_window function\n");
        return 0;
    }

    printf("creating window\n");

    gwin_handle_t temp_handle = NULL;

    if (!gwin_win32_create_window(&temp_handle)) {
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
            gwin_win32_destroy_window(temp_handle);

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
            gwin_win32_destroy_window(temp_handle);
                    
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

    gwin_win32_destroy_window(handle);

    printf("removing window...\n");
    remove_window_from_buffer(handle);
        

    return 0;
}

/**
 * @brief Paints a gVG buffer to a window.
 * @param handle The gWin handle of the window.
 * @param buffer The gVG buffer to paint.
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
void gwin_paint_gvg(gwin_handle_t handle, struct gvg_buffer_t buffer) {

    HDC hdc = GetDC(handle);

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = buffer.width;
    bmi.bmiHeader.biHeight = -buffer.height; // negative height for top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;

    SetDIBitsToDevice(hdc, 0, 0, buffer.width, buffer.height, 0, 0, 0, buffer.height, buffer.data, &bmi, DIB_RGB_COLORS);

    ReleaseDC(handle, hdc);
}

/**
 * @brief Tries to initialize gWin for win32.
 * @return 1 if the initialization succeeds, 0 if it fails.
 */
int gwin_win32_init() {

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc   = WindowProcedure;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    if (RegisterClass(&wc) == NULL) {
        DWORD errorCode = GetLastError();
         printf("GWIN_ERROR: failed to register Win32 window class. Error code: %lu\n", errorCode);
        printf("GWIN_ERROR: failed to register Win32 window class.\n");
        return 0;
    }

    return 1;


}

/**
 * @brief Finalizes gWin for win32.
 */
void gwin_win32_final() {

}


/**
 * @brief Polls win32 events.
 */
void gwin_win32_poll_events() {
    BOOL bRet;
    while ( (bRet = GetMessage( &g_msg, NULL, 0, 0 )) != 0) {
        TranslateMessage(&g_msg);
        DispatchMessage(&g_msg);
    }
}

/**
 * @brief Creates an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to write the new window to.
 * @return 1 if the window was created successfully, otherwise 0.
 */
int gwin_win32_create_window(gwin_handle_t* handle, gwin_window_creation_options_t options) {
    *handle = CreateWindowExW(
        0,
        CLASS_NAME,
        L"",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    printf("WINDOW: %llu\n", *handle);
    ShowWindow(*handle, SW_SHOW);

    return *handle != NULL;
}



/**
 * @brief Destroys an Application Window.
 * If the window is successfully created, it will have an arbitrary size, title, location and title.
 * These should all be set explicitly.
 * @param handle A gWin handle to destroy. This should have been checked already by gWin.c
 * @return 1 if the window was destroyed successfully, otherwise 0.
 */
int gwin_win32_destroy_window(gwin_handle_t handle) {
    return DestroyWindow(handle) > 0;
}



LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { 

    switch (msg) { 

        case WM_DESTROY: {
            if (g_window_close_request_callback != NULL) {

                if (g_window_close_request_callback((uintptr_t)hwnd)) {

                    gwin_destroy_window((uintptr_t)hwnd);
                }
            } else {
                gwin_destroy_window((uintptr_t)hwnd);
            }
        }

        case WM_PAINT: {
            if (g_window_paint_request_callback != NULL) {
                g_window_paint_request_callback((uintptr_t)hwnd);
            }
        }

        case WM_DISPLAYCHANGE: {
            InvalidateRect(hwnd, NULL, true);
        }
        

        case WM_SIZE: {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);

            struct gwin_window_size_t size = {width, height};

            if (g_window_resized_callback != NULL) {
                g_window_resized_callback((uintptr_t)hwnd, size);
            }

            InvalidateRect(hwnd, NULL, true);
        }

    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

