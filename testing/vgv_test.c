#include <stdio.h>
#include <stdint.h>

#include <gVG/pipeline/bytegen/gvg_bytegen.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {

    clock_t t; 
    t = clock(); 
    uint32_t* bytes = gvg_path_gen_bytes(NULL, 100, 100);
    t = clock() - t; 
    printf("draw took %llu us to execute \n", t); 

    if (bytes != NULL) {
        stbi_write_png("bytegen_render.png", 100, 100, 4, bytes, 100 * sizeof(uint32_t));
    }


    return 0;
}