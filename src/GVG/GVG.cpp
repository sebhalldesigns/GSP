// GVG.h - GSP Vector Graphics library
// GVG is intended to be a lightweight implementation of the SVG format.

#include "GVG.h"

#include "Termsyn.h"

bool PixelBuffer_Alloc(PixelBuffer& buffer) {
    buffer.data = (uint32_t*)malloc(buffer.width*buffer.height*sizeof(uint32_t));
    return (buffer.data != NULL);
}

void PixelBuffer_Free(PixelBuffer& buffer) {
    free(buffer.data);
}

bool PixelBuffer_Resize(PixelBuffer& buffer) {
    buffer.data = (uint32_t*)realloc(buffer.data, buffer.width*buffer.height*sizeof(uint32_t));
    return (buffer.data != NULL);
}

void PixelBuffer_Fill(PixelBuffer& buffer, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    uint64_t x, y;
    y = 0;
    while (y < buffer.height) {
        x = 0;
        while (x < buffer.width) {
            buffer.data[y * buffer.width + x] = (a << 24) | (b << 16) | (g << 8) | r;
            x++;
        }
        y++;
    }
}


void PixelBuffer_PrintTermsynGlyph(PixelBuffer& buffer, uint64_t x, uint64_t y, uint8_t ch) {
    // Check if the glyph fits within the bounds of the buffer
    if (x + termsynWidth > buffer.width || y + termsynHeight > buffer.height) {
        return; // Glyph does not fit, so we can't print it
    }

    // Iterate over the rows of the bitmap
    for (uint64_t i = 0; i < termsynHeight; i++) {
        // Iterate over the columns of the bitmap
        for (uint64_t j = 0; j < termsynWidth; j++) {
            // Calculate the index of the pixel in the buffer
            uint64_t bufferIndex = (y + i) * buffer.width + (x + j);
            // Calculate the alpha value of the pixel from the bitmap
            uint8_t alpha = (termsynGlpyhs[ch][i] >> (termsynWidth - j - 1)) & 0x01;
            // If the alpha value is non-zero, set the corresponding pixel in the buffer
            if (alpha > 0) {
                // Calculate the color of the pixel based on the alpha value
                // For simplicity, let's set it to white (0xFFFFFF) with the alpha value as the alpha channel
                uint32_t color = (alpha << 24) | 0xFFFFFF;
                // Set the pixel in the buffer
                buffer.data[bufferIndex] = 0xFFFFFFFF;
            }
        }
    }
}

void PixelBuffer_PrintGlyph(PixelBuffer& buffer, uint64_t x, uint64_t y) {
    // Check if the glyph fits within the bounds of the buffer
    if (x + GLYPH_WIDTH > buffer.width || y + GLYPH_HEIGHT > buffer.height) {
        return; // Glyph does not fit, so we can't print it
    }

    // Iterate over the rows of the bitmap
    for (uint64_t i = 0; i < GLYPH_HEIGHT; i++) {
        // Iterate over the columns of the bitmap
        for (uint64_t j = 0; j < GLYPH_WIDTH; j++) {
            // Calculate the index of the pixel in the buffer
            uint64_t bufferIndex = (y + i) * buffer.width + (x + j);
            // Calculate the alpha value of the pixel from the bitmap
            uint8_t alpha = (bitmapA[i] >> (GLYPH_WIDTH - j - 1)) & 0x01;
            // If the alpha value is non-zero, set the corresponding pixel in the buffer
            if (alpha > 0) {
                // Calculate the color of the pixel based on the alpha value
                // For simplicity, let's set it to white (0xFFFFFF) with the alpha value as the alpha channel
                uint32_t color = (alpha << 24) | 0xFFFFFF;
                // Set the pixel in the buffer
                buffer.data[bufferIndex] = color;
            }
        }
    }
}