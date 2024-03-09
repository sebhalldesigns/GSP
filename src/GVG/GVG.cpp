// GVG.h - GSP Vector Graphics library
// GVG is intended to be a lightweight implementation of the SVG format.

#include "GVG.h"

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