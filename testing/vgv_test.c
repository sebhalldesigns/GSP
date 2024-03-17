#include <stdio.h>
#include <stdint.h>

#include <gVG/gVG.h>
#include <gVG/pipeline/bytegen/gvg_bytegen.h>
#include <gVG/pipeline/combine/gvg_combine.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {

    clock_t t; 
    t = clock(); 
    uint32_t* bytes = gvg_path_gen_bytes(NULL, 100, 100);
    t = clock() - t; 
    printf("draw took %llu us to execute \n", t); 

    if (bytes != NULL) {
        stbi_write_png("bytegen_render.png", 100, 100, 4, bytes, 100 * sizeof(uint32_t));
    }

    struct gvg_buffer_t destination_buffer = gvg_buffer_alloc(500, 500);

    struct gvg_color_t destination_color = {
        1.0f, 1.0f, 1.0f, 1.0f
    };

    gvg_buffer_fill(destination_buffer, destination_color);

    struct gvg_buffer_t green = gvg_buffer_alloc(100, 100);

    struct gvg_color_t green_color = {
        1.0f, 1.0f, 0.0f, 0.75f
    };

    gvg_buffer_fill(green, green_color);

    gvg_bitmap_point_t point = { 10, 10};

    gvg_combine_buffers(green.data, 100, 100, destination_buffer.data, point, 500, 500);


    struct gvg_buffer_t red = gvg_buffer_alloc(100, 100);

    struct gvg_color_t red_color = {
        0.0f, 1.0f, 1.0f, 0.75f
    };
    gvg_bitmap_point_t point2 = { 60, 60};

    gvg_buffer_fill(red, red_color);
    gvg_combine_buffers(red.data, 100, 100, destination_buffer.data, point2, 500, 500);





    stbi_write_png("bytegen_render.png", 500, 500, 4, destination_buffer.data, 500 * sizeof(uint32_t));



    return 0;
}