#include <stdio.h>

#include <gWin/gWin.h>

void launched() {

    printf("LAUNCHED\n");

    gwin_handle_t* window;
    gwin_create_window(window);
}

int main() {


    printf("HELLO WINDOWTEST\n");

    

    gwin_set_launch_callback(launched);

    return gwin_run();
}