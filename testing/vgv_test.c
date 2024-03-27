#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include <gVG/gVG.h>
#include <gVG/pipeline/bytegen/gvg_bytegen.h>
#include <gVG/pipeline/combine/gvg_combine.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <ft2build.h>
#include FT_FREETYPE_H

// Function to generate an RGBA bitmap for a specific glyph
// Note that the generated bitmap is imperfect and ignores the baseline.
uint32_t* generate_rgba_bitmap(const char* font_file, uint32_t font_size, uint32_t glyph_char, int* width, int* height) {
    FT_Library ft_library;
    FT_Face ft_face;
    FT_Error error;

    // Initialize FreeType library
    error = FT_Init_FreeType(&ft_library);
    if (error) {
        fprintf(stderr, "Error initializing FreeType library\n");
        return NULL;
    }

    // Load the font file
    error = FT_New_Face(ft_library, font_file, 0, &ft_face);
    if (error == FT_Err_Unknown_File_Format) {
        fprintf(stderr, "Unsupported font format\n");
        return NULL;
    } else if (error) {
        fprintf(stderr, "Error loading font file\n");
        return NULL;
    }

    // Set the font size
    error = FT_Set_Pixel_Sizes(ft_face, 0, font_size);
    if (error) {
        fprintf(stderr, "Error setting font size\n");
        return NULL;
    }

    // Load the glyph for the specified character
    error = FT_Load_Char(ft_face, glyph_char, FT_LOAD_RENDER);
    if (error) {
        fprintf(stderr, "Error loading glyph\n");
        return NULL;
    }

    // Get glyph bitmap
    FT_Bitmap ft_bitmap = ft_face->glyph->bitmap;

    // Create RGBA bitmap
    *width = ft_bitmap.width;
    *height = ft_bitmap.rows;
    
    uint32_t* rgba_bitmap = malloc(*width * *height * sizeof(uint32_t));
    if (!rgba_bitmap) {
        fprintf(stderr, "Error allocating memory for RGBA bitmap\n");
        return NULL;
    }

    // Convert FT_Bitmap to RGBA bitmap
    for (int y = 0; y < ft_bitmap.rows; y++) {
        for (int x = 0; x < ft_bitmap.width; x++) {
            uint8_t pixel = ft_bitmap.buffer[y * ft_bitmap.width + x];
            rgba_bitmap[y * ft_bitmap.width + x] = (pixel << 24) | (0x00 << 16) | (0x00 << 8) | 0x00;
        }
    }

    // Clean up FreeType resources
    FT_Done_Face(ft_face);
    FT_Done_FreeType(ft_library);

    return rgba_bitmap;
}


int main() {

    clock_t t; 
    t = clock(); 
    uint32_t* bytes = gvg_path_gen_bytes(NULL, 100, 100);
    t = clock() - t; 
    printf("draw took %llu us to execute \n", t); 

    if (bytes != NULL) {
        stbi_write_png("bytegen_render.png", 100, 100, 4, bytes, 100 * sizeof(uint32_t));
    }

    struct gvg_buffer_t destination_buffer = gvg_buffer_alloc(500, 500);

    struct gvg_color_t destination_color = {
        1.0f, 1.0f, 1.0f, 1.0f
    };

    gvg_buffer_fill(destination_buffer, destination_color);

    struct gvg_buffer_t green = gvg_buffer_alloc(100, 100);

    struct gvg_color_t green_color = {
        1.0f, 1.0f, 0.0f, 1.0f
    };

    gvg_buffer_fill(green, green_color);

    gvg_bitmap_point_t point = { 10, 10};

    gvg_combine_buffers(green.data, 100, 100, destination_buffer.data, point, 500, 500);


    struct gvg_buffer_t red = gvg_buffer_alloc(100, 100);

    struct gvg_color_t red_color = {
        0.0f, 1.0f, 1.0f, 0.1f
    };
    gvg_bitmap_point_t point2 = { 60, 60};

    gvg_buffer_fill(red, red_color);
    gvg_combine_buffers(red.data, 100, 100, destination_buffer.data, point2, 500, 500);


    const char* font_file = "Inter-Regular.ttf"; // Change this to your font file
    uint32_t font_size = 24;
    uint32_t glyph_char = '0'; // Character for which glyph to be generated

    int width, height;
    uint32_t* rgba_bitmap = generate_rgba_bitmap(font_file, font_size, glyph_char, &width, &height);
    if (rgba_bitmap) {

        gvg_bitmap_point_t point3 = { 2, 2 };
        printf("Generated RGBA bitmap for glyph '%c': width = %d, height = %d\n", glyph_char, width, height);
        // Use rgba_bitmap as needed
        gvg_combine_buffers(rgba_bitmap, width, height, destination_buffer.data, point3, 500, 500);

        // Free memory
        free(rgba_bitmap);
    } else {
        fprintf(stderr, "Error generating RGBA bitmap for glyph '%c'\n", glyph_char);
    }

    gvg_bitmap_point_t point3 = { 400, 400 };
    gvg_combine_buffers(bytes, 100, 100, destination_buffer.data, point3, 500, 500);    

    stbi_write_png("bytegen_render.png", 500, 500, 4, destination_buffer.data, 500 * sizeof(uint32_t));



    return 0;
}