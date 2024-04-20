#ifndef GSP_SYSTEM_H
#define GSP_SYSTEM_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// APP LIFECYCLE

// version struct
typedef struct {
    uint32_t major;
    uint32_t minor;
    uint32_t build;
} GspVersion;

// callback for when application is launched
typedef void (*GspAppLaunchCallback)();

// app creation info
typedef struct {
    char* name;
    char* developer;
    GspVersion version;
    GspAppLaunchCallback launch_callback;
    bool background;
} GspAppInfo;

// main run function
int gsp_run(GspAppInfo app_info);

// WINDOWING

typedef struct {
    int32_t width;
    int32_t height;
} GspWindowSize;

typedef uintptr_t GspWindowHandle;
typedef void (*GspWindowResizedCallback)(GspWindowHandle window, GspWindowSize new_size);
typedef GspWindowSize (*GspWindowWillResizeCallback)(GspWindowHandle window, GspWindowSize requested_size);
typedef void (*GspWindowClosedCallback)(GspWindowHandle window);
typedef bool (*GspWindowWillCloseCallback)(GspWindowHandle window);

// window info, used for creation
typedef struct {
    char* title;
    int32_t width;
    int32_t height;
    int32_t x;
    int32_t y;
    bool hidden;
} GspWindowInfo;

// create window function, returns a handle of 0 if creation failed
GspWindowHandle gsp_create_window(GspWindowInfo creation_info);

void gsp_set_window_resized_callback(GspWindowHandle window, GspWindowResizedCallback resize_callback);
void gsp_set_window_will_resize_callback(GspWindowHandle window, GspWindowWillResizeCallback will_resize_callback);
void gsp_set_window_closed_callback(GspWindowHandle window, GspWindowClosedCallback closed_callback);
void gsp_set_window_will_close_callback(GspWindowHandle window, GspWindowWillCloseCallback will_close_callback);



#endif // GSP_SYSTEM_H
