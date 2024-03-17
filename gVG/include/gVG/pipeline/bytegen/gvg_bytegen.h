/**
 * @file gvg_bytegen.h
 * @brief This file contains the gVG bytegen (rasterizer) interface.
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
    uint8_t code;
    uint8_t num_params;
    float* params;
} gvg_path_cmd_t;

typedef struct {
    size_t num_cmds;
    gvg_path_cmd_t* cmds;
} gvg_path_t;

// NOTES:
// The gVG 'path' in this situation refers to a set of pre-calculated lines.
// These should be generated from primitives and svg-style curves.
// This particular function could be implemented as a GPU render in isolation,
// but testing will identify how much benefit that brings.

uint32_t* gvg_path_gen_bytes(gvg_path_t* path, size_t width, size_t height);

