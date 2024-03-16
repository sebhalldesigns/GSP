#include <stdio.h>
#include <time.h>

#include <gWin/gWin.h>

#include <gVG/gVG.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

size_t window_width = 0;
size_t window_height = 0;

void launched() {
    printf("my app launched!\n");

    gwin_handle_t window;

    if (gwin_create_window(&window)) {
        printf("window created!\n");
    } else {
        printf("window creation failed\n");
    }
}

void paint(gwin_handle_t window) {
        
    printf("PAINT\n");

    struct gvg_buffer_t buffer = gvg_buffer_alloc(window_width, window_height); 
    struct gvg_color_t color = {
        0.0f, 1.0f, 1.0f, 1.0f
    };

    gvg_buffer_fill(buffer, color);

    gwin_paint_gvg(window, buffer);

    gvg_buffer_free(buffer);
}

void resize(gwin_handle_t window, struct gwin_window_size_t size) {
    printf("RESIZE\n");
    window_width = size.width;
    window_height = size.height;
}

int main() {

    struct gvg_buffer_t buffer = gvg_buffer_alloc(100, 100); 
    struct gvg_color_t color = {
        0.0f, 1.0f, 1.0f, 1.0f
    };

    gvg_buffer_fill(buffer, color);
    stbi_write_png("image.png", buffer.width, buffer.height, 4, buffer.data, buffer.width * sizeof(uint32_t));

    gwin_set_launch_callback(launched);
    gwin_set_window_paint_request_callback(paint);
    gwin_set_window_resized_callback(resize);

    return gwin_run();
}


