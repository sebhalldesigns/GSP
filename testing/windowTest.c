#include <stdio.h>

#include <gWin/gWin.h>

void WindowResized(gwin_handle_t window, gwin_window_size_t size) {

    printf("WINDOW RESIZED %d %d!\n", size.width, size.height);
}

void launched() {

    printf("LAUNCHED\n");

    printf("my app launched!\n");

    gwin_handle_t window;

    gwin_window_creation_options_t options;
    options.center_screen = true;
    options.size = (gwin_window_size_t) { 1000, 750 };
    options.title = "MAIN WINDOW";

    if (gwin_create_window(&window, options)) {
        printf("window created!\n");
        gwin_set_window_resized_callback(WindowResized);
    } else {
        printf("window creation failed\n");
    }
}

int main() {


    printf("HELLO WINDOWTEST\n");

    

    gwin_set_launch_callback(launched);

    return gwin_run();
}