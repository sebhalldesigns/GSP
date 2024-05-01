/**
 * @file gvg_combine.h
 * @brief This file contains the gVG compositor interface.
 * It is not intended for direct interaction by those using gVG. Instead simply import gVG.h.
 * This file is implemented separately for Windows and POSIX systems.
 * 
 * @date 2024/03/17
 * @author Seb Hall
 * @copyright Copyright (c) 2024
 * OpenGSP is distributed under the MIT License. See the LICENSE file for more details.
 */


#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
} gvg_bitmap_region_t;

typedef struct {
    size_t x;
    size_t y;
} gvg_bitmap_point_t;

void gvg_combine_buffers(uint32_t* source, 
    size_t source_width,
    size_t source_height, 
    uint32_t* destination, 
    gvg_bitmap_point_t location,
    size_t destination_width,
    size_t destination_height);

