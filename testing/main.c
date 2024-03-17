#include <stdio.h>
#include <time.h>

#include <gWin/gWin.h>

#include <gVG/gVG.h>

//#include <Windows.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

size_t window_width = 0;
size_t window_height = 0;

struct gvg_buffer_t buffer;

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

    /* LARGE_INTEGER frequency, start, end;
    double elapsed;

    // Get the frequency of the performance counter
    QueryPerformanceFrequency(&frequency);

    // Get the starting time
    QueryPerformanceCounter(&start);

    buffer = gvg_buffer_resize(buffer, window_width, window_height);

    // Get the ending time
    QueryPerformanceCounter(&end);

    // Calculate the elapsed time in seconds
    elapsed = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    // Convert elapsed time to microseconds
    elapsed *= 1000000;

    printf("alloc took %.3f us to execute\n", elapsed);*/

    clock_t t; 
    t = clock(); 
        
    //struct gvg_buffer_t buffer = gvg_buffer_alloc(window_width, window_height); 
    buffer = gvg_buffer_resize(buffer, window_width, window_height);
    t = clock() - t; 
    printf("alloc took %llu us to execute \n", t); 

    struct gvg_color_t color = {
        0.0f, 1.0f, 1.0f, 1.0f
    };

    t = clock(); 
    gvg_buffer_fill(buffer, color);
    
    t = clock() - t; 
    printf("fill took %llu us to execute \n", t); 

    

    gwin_paint_gvg(window, buffer);

    //gvg_buffer_free(buffer);

    

    
}

void resize(gwin_handle_t window, struct gwin_window_size_t size) {
    //printf("RESIZE\n");
    window_width = size.width;
    window_height = size.height;
}

int main() {

    gvg_init();

    //struct gvg_buffer_t buffer = gvg_buffer_alloc(100, 100); 
    /*struct gvg_color_t color = {
        0.0f, 1.0f, 1.0f, 1.0f
    };*/

    // Your code here
    buffer = gvg_buffer_alloc(100, 100); 

    //gvg_buffer_fill(buffer, color);
    //stbi_write_png("image.png", buffer.width, buffer.height, 4, buffer.data, buffer.width * sizeof(uint32_t));

    gwin_set_launch_callback(launched);
    gwin_set_window_paint_request_callback(paint);
    gwin_set_window_resized_callback(resize);

    return gwin_run();
}


