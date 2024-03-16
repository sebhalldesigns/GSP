#include <stdio.h>
#include <gWin/gWin.h>

void launched() {
    printf("my app launched!\n");

    gwin_handle_t window;

    if (gwin_create_window(&window)) {
        printf("window created!\n");
    } else {
        printf("window creation failed\n");
    }
}

int main() {

    gwin_set_launch_callback(launched);

    return gwin_run();
}


