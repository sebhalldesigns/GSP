#include "gvg.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>

int main() {

    VgEngineState state = vg_init();
    vg_reset(state);

    

    //char* msg = send_msg();

    //printf(msg);
    //printf("\n");

    //struct VGPixelBuffer buffer = vgpixelbuffer_alloc(100, 100);

    //stbi_write_png("image.png", buffer.width, buffer.height, 4, buffer.data, buffer.width * sizeof(uint32_t));

    return 0;
}