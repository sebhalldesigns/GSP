/**
 * @file gVG.h
 * @brief This file contains the gVG module interface. It should be included when using gVG, and other header files in gVG ignored.
 */

#ifndef GVG_H
#define GVG_H

#include <stddef.h>
#include <stdint.h>

/**
 * @typedef gVG Pixel Buffer type
 * @brief represents a rectangular buffer with a 32-bit pixel depth.
 */
typedef struct gvg_buffer_t {
    size_t width;
    size_t height;
    uint32_t* data;
};

/**
 * @typedef gVG Color type
 * @brief represents a 4-channel color.
 */
typedef struct gvg_color_t {
    float red;
    float green;
    float blue;
    float alpha;
};


/**
 * @brief Tries to allocate a new pixel buffer, filled with a clear color.
 * @param width The width of the buffer to allocate in pixels.
 * @param height The height of the buffer to allocate in pixels.
 * @return The allocated buffer with fields filled in. 
 * If the data section is NULL, the buffer failed to allocate.
 */
struct gvg_buffer_t gvg_buffer_alloc(size_t width, size_t height);

/**
 * @brief Tries to resize the given buffer.
 * @param buffer The buffer to try and resize.
 * @param width The desired width in pixels.
 * @param height The desired height in pixels.
 * @return The resized buffer with fields filled in. 
 * If the data section is NULL, the buffer failed to allocate.
 */
struct gvg_buffer_t gvg_buffer_resize(struct gvg_buffer_t buffer, size_t width, size_t height);


/**
 * @brief Frees a buffer.
 * @param buffer The buffer to free.
 */
void gvg_buffer_free(struct gvg_buffer_t buffer);

/**
 * @brief Fills a buffer.
 * @param buffer The buffer to fill.
 * @param color The color to fill it with.
 */
void gvg_buffer_fill(struct gvg_buffer_t buffer, struct gvg_color_t color);

#endif // GVG_H