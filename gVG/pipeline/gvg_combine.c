#include <gVG/pipeline/combine/gvg_combine.h>
#include <gVG/pipeline/common/gvg_common.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Function to blend two colors using alpha blending

void gvg_combine_buffers(uint32_t* source, 
    size_t source_width,
    size_t source_height, 
    uint32_t* destination, 
    gvg_bitmap_point_t location,
    size_t destination_width,
    size_t destination_height) {
    
    if (source == NULL || destination == NULL) {
        printf("GVG_WARNING: invalid buffer(s) passed to combine function\n");
    }

    for (size_t y = 0; y < source_width; y++) {
        for (size_t x = 0; x < source_height; x++) {

            // Get the source and destination colors
            uint32_t src_color = source[y * source_width + x];
            uint32_t dst_color = destination[(location.y + y) * destination_width + location.x + x];

            // Blend the colors and store the result in the destination buffer
            destination[(location.y + y) * destination_width + location.x + x] = gvg_combine_colors(src_color, dst_color);

        }
    }

}