#include <stdio.h>
#include <gWin/gWin.h>

#include <gVG/gVG.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


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

    struct gvg_buffer_t buffer = gvg_buffer_alloc(100, 100); 
    struct gvg_color_t color = {
        0.0f, 1.0f, 1.0f, 1.0f
    };

    gvg_buffer_fill(buffer, color);
    stbi_write_png("image.png", buffer.width, buffer.height, 4, buffer.data, buffer.width * sizeof(uint32_t));

    //gwin_set_launch_callback(launched);

    return gwin_run();
}


