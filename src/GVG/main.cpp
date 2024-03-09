#include "GVG.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdio.h>

int main() {

    PixelBuffer buffer;
    buffer.width = 500;
    buffer.height = 20;


    PixelBuffer_Alloc(buffer);
    

    // r g b
    PixelBuffer_Fill(buffer, 0x00, 0x00, 0xFF, 0xFF);
    uint8_t text[12] = { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'};
    for (int g = 0; g < 12; g++) {
        PixelBuffer_PrintTermsynGlyph(buffer, 1 + (7*g), 1, text[g]);
    }

    stbi_write_png("image.png", buffer.width, buffer.height, 4, buffer.data, buffer.width * sizeof(uint32_t));

    printf("test\n");
    return 0;
}