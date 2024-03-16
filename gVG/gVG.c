#include <gVG/gVG.h>

#include <stdio.h> // for printf etc
#include <stdlib.h> // for malloc etc

/**
 * @brief Tries to allocate a new pixel buffer, filled with white.
 * @param width The width of the buffer to allocate in pixels.
 * @param height The height of the buffer to allocate in pixels.
 * @return The allocated buffer with fields filled in. 
 * If the data section is NULL, the buffer failed to allocate.
 */
struct gvg_buffer_t gvg_buffer_alloc(size_t width, size_t height) {

    struct gvg_buffer_t temp_buffer;
    temp_buffer.width = width;
    temp_buffer.height = height;
    temp_buffer.data = NULL;

    if (width > SIZE_MAX / height) {
        printf("GVG_WARNING: ignoring buffer allocation request because the size would be too big\n");
        return temp_buffer;
    }
    
    temp_buffer.data = (uint32_t*)malloc(width*height*sizeof(uint32_t));

    if (temp_buffer.data != NULL) {
        temp_buffer.width = width;
        temp_buffer.height = height;
    } else {
        printf("GVG_ERROR: buffer allocation failed.\n");
        return temp_buffer;
    }

    return temp_buffer;
}

/**
 * @brief Frees a buffer.
 * @param buffer The buffer to free.
 */
void gvg_buffer_free(struct gvg_buffer_t buffer) {
    free(buffer.data);
    buffer.width = 0;
    buffer.height = 0;
}

/**
 * @brief Fills a buffer.
 * @param buffer The buffer to fill.
 * @param color The color to fill it with.
 */
void gvg_buffer_fill(struct gvg_buffer_t buffer, struct gvg_color_t color) {

    if (buffer.data == NULL) {
        printf("GVG_WARNING: ignoring buffer fill request because the buffer is invalid\n");
        return;
    }

    uint32_t pixel_color = ((uint8_t)(color.alpha * 255) << 24) 
    | ((uint8_t)(color.blue * 255) << 16) 
    | ((uint8_t)(color.green * 255) << 8) 
    | (uint8_t)(color.red * 255);

    for (size_t i = 0; i < buffer.width * buffer.height; i++) {
        buffer.data[i] = pixel_color;
    }

    printf("FILLING %x\n", pixel_color);

}