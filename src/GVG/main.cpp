#include "GVG.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdio.h>

int main() {

    PixelBuffer buffer;
    buffer.width = 100;
    buffer.height = 100;


    PixelBuffer_Alloc(buffer);

    
    // r g b
    PixelBuffer_Fill(buffer, 0x00, 0x00, 0xFF, 0xFF);


    stbi_write_png("image.png", buffer.width, buffer.height, 4, buffer.data, buffer.width * sizeof(uint32_t));

    printf("test\n");
    return 0;
}