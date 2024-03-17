#include <gVG/pipeline/bytegen/gvg_bytegen.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void draw_line(float x0, float y0, float x1, float y1, uint32_t* pixel_buffer, size_t width, size_t height, uint32_t color);

uint32_t* gvg_path_gen_bytes(gvg_path_t* path, size_t width, size_t height) {;


    // ALLOCATE BUFFER

    uint32_t* bytes = NULL;

    if (width > SIZE_MAX / height) {
        printf("GVG_WARNING: ignoring buffer bytegen request because the size would be too big\n");
        return bytes;
    }

    if (width * height == 0) {
        printf("GVG_WARNING: ignoring buffer bytegen request because the size is zero\n");
        return bytes;
    }
    
    bytes = (uint32_t*)malloc(width*height*sizeof(uint32_t));

    if (bytes == NULL)  {
        printf("GVG_ERROR: buffer allocation failed.\n");
        return bytes;
    }

    for (size_t i = 0; i < width * height; i++) {
        bytes[i] = 0xFFF0F00F;
    }


    //draw_line(10.0, 10.0, 90.0, 90.0, bytes, width, height, 0xFFFFFFFF);
    draw_line(10.0, 10.0, 900.0, 900.0, bytes, width, height, 0xFFFFFFFF);


    return bytes;
}

// Function to draw a line using Xiaolin Wu's algorithm

float ipart(float x) {
    return floor(x);
}

float fpart(float x) {
    return x - ipart(x);
}

float rfpart(float x) {
    return 1 - fpart(x);
}


uint32_t color_gen(float red, float green, float blue, float alpha) {

    
    uint32_t output = ((uint8_t)(alpha * 255) << 24) 
            | ((uint8_t)(blue * 255) << 16) 
            | ((uint8_t)(green * 255) << 8) 
            | (uint8_t)(red * 255);
    printf("COLOR WITH ALPHA OF %f MAKES %x\n", alpha, output);
    return output;
}

// Function to blend colors using alpha blending
uint32_t blend_colors(uint32_t old_color, uint32_t new_color) {
    

    // Extract the alpha channel from the new color
    uint8_t alpha = (new_color >> 24) & 0xFF;

    if (alpha == 0) {
        // If alpha is zero, no blending occurs, return the original color
        return old_color;
    }

    printf("COLOR WITH NEW ALPHA OF %u\n", alpha);


    // Extract the RGB channels from both colors
    uint8_t old_red = (old_color >> 16) & 0xFF;
    uint8_t old_green = (old_color >> 8) & 0xFF;
    uint8_t old_blue = old_color & 0xFF;
    uint8_t new_red = (new_color >> 16) & 0xFF;
    uint8_t new_green = (new_color >> 8) & 0xFF;
    uint8_t new_blue = new_color & 0xFF;

    // Calculate the blended color components
    uint8_t blended_red = (new_red * alpha + old_red * (255 - alpha)) / 255;
    uint8_t blended_green = (new_green * alpha + old_green * (255 - alpha)) / 255;
    uint8_t blended_blue = (new_blue * alpha + old_blue * (255 - alpha)) / 255;


    // Combine the blended color components into a single uint32_t value
    uint32_t output =  (alpha << 24) | (blended_red << 16) | (blended_green << 8) | blended_blue;
    printf("%x BLENDED WITH %x TO %x\n", old_color, new_color, output );

    return output;
}

void draw_pixel(uint32_t* pixel_buffer, size_t x, size_t y, size_t width, size_t height, uint32_t color) {
    // TODO check it's not trying to access a bad address
    if (x > width || y > height) {
        return;
    }

    uint32_t old_color = pixel_buffer[y * width + x];
    pixel_buffer[y * width + x] = blend_colors(old_color, color);
    printf("%x BLENDED TO %x\n", old_color, pixel_buffer[y * width + x]);
}
void draw_line(float x0, float y0, float x1, float y1, uint32_t* pixel_buffer, size_t width, size_t height, uint32_t color) {

    float blue = 1.0f;
    float green = 1.0f;
    float red = 1.0f;
    float alpha = 0.5f;
    
    // true if gradient is positive, false if it is negative
    bool steep = fabs(y1 - y0) > fabs(x1 - x0);

    // swap co-ordinates if positive gradient
    if (steep) {
        float temp = x0;
        x0 = y0;
        y0 = temp;

        temp = x1;
        x1 = y1;
        y1 = temp;
    }

    // swap co-ordinates if the line is backwards in x
    if (x0 > x1) {
        float temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;  
    }

    float dx = x1 - x0;
    float dy = y1 - y0;

    float gradient = dy/dx;
    if (dx == 0.0f) {
        gradient = 1.0f;
    }


    // DRAW FIRST ENDPOINT 

    float xend = round(x0);
    float yend = y0 + gradient * (xend - x0);
    float xgap = rfpart(x0 + 0.5f);

    size_t xpxl1 = xend; 
    size_t ypxl1 = ipart(yend);

    if (steep) {

        draw_pixel(pixel_buffer, ypxl1, xpxl1, width, height, color_gen(red, green, blue, alpha*rfpart(yend) * xgap));
        draw_pixel(pixel_buffer, ypxl1 + 1, xpxl1, width, height, color_gen(red, green, blue, alpha*fpart(yend) * xgap));

    } else {

        draw_pixel(pixel_buffer, xpxl1, ypxl1, width, height, color_gen(red, green, blue, alpha*rfpart(yend) * xgap));
        draw_pixel(pixel_buffer, xpxl1, ypxl1 + 1, width, height, color_gen(red, green, blue, alpha*fpart(yend) * xgap));
    }
    
    float intery = yend + gradient;


    // DRAW SECOND ENDPOINT 

    xend = round(x1);
    yend = y1 + gradient * (xend - x1);
    xgap = fpart(x1 + 0.5f);

    size_t xpxl2 = xend; 
    size_t ypxl2 = ipart(yend);

    if (steep) {

        draw_pixel(pixel_buffer, ypxl2, xpxl2, width, height, color_gen(red, green, blue, alpha*rfpart(yend) * xgap));
        draw_pixel(pixel_buffer, ypxl2 + 1, xpxl2, width, height, color_gen(red, green, blue, alpha*fpart(yend) * xgap));

    } else {

        draw_pixel(pixel_buffer, xpxl2, ypxl2, width, height, color_gen(red, green, blue, alpha*rfpart(yend) * xgap));
        draw_pixel(pixel_buffer, xpxl2, ypxl2 + 1, width, height, color_gen(red, green, blue, alpha*fpart(yend) * xgap));
    }

    // MAIN LOOP

    if (steep) {
        for (size_t x = xpxl1 + 1; x < xpxl2; x++) {
            draw_pixel(pixel_buffer, (size_t)ipart(intery), x, width, height, color_gen(red, green, blue, alpha*rfpart(intery)));
            draw_pixel(pixel_buffer, (size_t)ipart(intery)+1, x, width, height, color_gen(red, green, blue, alpha*fpart(intery)));
            intery += gradient;
            if (intery > width || x > height) {
                return;
            }
        }
    } else {
        for (size_t x = xpxl1 + 1; x < xpxl2; x++) {
            draw_pixel(pixel_buffer, x, (size_t)ipart(intery), width, height, color_gen(red, green, blue, alpha*rfpart(intery)));
            draw_pixel(pixel_buffer, x, (size_t)ipart(intery)+1, width, height, color_gen(red, green, blue, alpha*fpart(intery)));
            intery += gradient;
            if (x > width || intery > height) {
                return;
            }
        }
    }
}