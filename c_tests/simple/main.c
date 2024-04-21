#include "gsp_system.h"
#include <stdio.h>

bool has_closed_once = false;

bool window_will_close(GspWindowHandle window) {

    printf("WILL CLOSE FROM C\n");

    if (!has_closed_once) {
        has_closed_once = true;
        return false;
    }

    return true;
}

void window_closed(GspWindowHandle window) {
    printf("WINDOW CLOSED TO C\n");
}

GspWindowSize window_will_resize(GspWindowHandle window, GspWindowSize size) {
    printf("WINDOW WILL RESIZE FROM C\n");
    int snap_size = 50;
size.width = (size.width + snap_size) / snap_size * snap_size;
  size.height = (size.height + snap_size) / snap_size * snap_size;
    return size;
}

void window_resized(GspWindowHandle window, GspWindowSize size) {
    printf("RESIZED TO C %d %d\n", size.width, size.height);
}

void run_callback() {
    printf("CALLBACK FROM C\n");

    GspWindowInfo info = {
        "My Window from C",
        800,
        600,
        0,
        0,
        false
    };

    GspWindowHandle window = gsp_create_window(info);

    gsp_set_window_resized_callback(window, window_resized);
    gsp_set_window_will_resize_callback(window, window_will_resize);
   gsp_set_window_closed_callback(window, window_closed);
     gsp_set_window_will_close_callback(window, window_will_close);
}

int main() {

    GspAppInfo app = {
        "Test App",
        "Seb Hall",
        { 0, 0, 1 },
        run_callback
    };

    return gsp_run(app);
}
