// GVG.h - GSP Vector Graphics library
// GVG is intended to be a lightweight implementation of the SVG format.

#ifndef GVG_H
#define GVG_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


struct PixelBuffer {
    uint64_t width;
    uint64_t height;
    uint32_t* data;
};

struct FallbackGlyph {
    uint32_t data[96];
};

#define GLYPH_WIDTH 7
#define GLYPH_HEIGHT 14

const uint8_t bitmapA[GLYPH_HEIGHT] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111000,
    0b01000100,
    0b01000100,
    0b01000100,
    0b01111100,
    0b01000100,
    0b01000100,
    0b01000100,
    0b00000000,
    0b00000000,
    0b00000000
};

const uint8_t bitmapB[GLYPH_HEIGHT] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111000,
    0b01000100,
    0b01000100,
    0b01000100,
    0b01111100,
    0b01000100,
    0b01000100,
    0b01000100,
    0b00000000,
    0b00000000,
    0b00000000
};


struct Color {
    uint8_t r; // Red component
    uint8_t g; // Green component
    uint8_t b; // Blue component
    uint8_t a; // Alpha component (transparency)
};

// allocate buffer for pixel data based off parameters
bool PixelBuffer_Alloc(PixelBuffer& buffer);

// frees buffer data
void PixelBuffer_Free(PixelBuffer& buffer);

// resizes buffer
bool PixelBuffer_Resize(PixelBuffer& buffer);

void PixelBuffer_Fill(PixelBuffer& buffer, uint8_t r, uint8_t g, uint8_t b, uint8_t a);


void PixelBuffer_PrintTermsynGlyph(PixelBuffer& buffer, uint64_t x, uint64_t y, uint8_t ch);

#endif // GVG_H