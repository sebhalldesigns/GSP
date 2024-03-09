// GVG.h - GSP Vector Graphics library
// GVG is intended to be a lightweight implementation of the SVG format.

#ifndef GVG_H
#define GVG_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


// assume 4 channels per pixel
struct PixelBuffer {
    uint64_t width;
    uint64_t height;
    uint32_t* data;
};

// allocate buffer for pixel data based off parameters
bool PixelBuffer_Alloc(PixelBuffer& buffer);

// frees buffer data
void PixelBuffer_Free(PixelBuffer& buffer);

// resizes buffer
bool PixelBuffer_Resize(PixelBuffer& buffer);

void PixelBuffer_Fill(PixelBuffer& buffer, uint8_t r, uint8_t g, uint8_t b, uint8_t a);


#endif // GVG_H