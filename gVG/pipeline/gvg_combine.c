#include <gVG/pipeline/combine/gvg_combine.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Function to blend two colors using alpha blending

uint32_t blend_colors2(uint32_t src_color, uint32_t dst_color) {

  // Get the alpha channel of the source color
  uint8_t src_alpha = (src_color & 0xFF000000) >> 24;

  // Invert the source alpha for smoother blending
  uint8_t inv_src_alpha = 255 - src_alpha;

  // Extract the red, green, and blue components from both colors
  uint8_t src_red = (src_color & 0x00FF0000) >> 16;
  uint8_t src_green = (src_color & 0x0000FF00) >> 8;
  uint8_t src_blue = (src_color & 0x000000FF);

  uint8_t dst_red = (dst_color & 0x00FF0000) >> 16;
  uint8_t dst_green = (dst_color & 0x0000FF00) >> 8;
  uint8_t dst_blue = (dst_color & 0x000000FF);

  // Blend the red, green, and blue components using alpha blending
  uint8_t blended_red = (src_red * src_alpha + dst_red * inv_src_alpha) / 255;
  uint8_t blended_green = (src_green * src_alpha + dst_green * inv_src_alpha) / 255;
  uint8_t blended_blue = (src_blue * src_alpha + dst_blue * inv_src_alpha) / 255;

  // Pack the blended components back into a uint32_t color
  return (src_alpha << 24) | (blended_red << 16) | (blended_green << 8) | blended_blue;
}

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
            destination[(location.y + y) * destination_width + location.x + x] = blend_colors2(src_color, dst_color);

        }
    }

}