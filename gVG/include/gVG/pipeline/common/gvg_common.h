/**
 * @file gvg_common.h
 * @brief This file contains definitions for commonly used vector graphics functions.
 * It is not intended for direct interaction by those using gVG. Instead simply import gVG.h.
 * This file is implemented separately for Windows and POSIX systems.
 * 
 * @date 2024/03/17
 * @author Seb Hall
 * @copyright Copyright (c) 2024
 * OpenGSP is distributed under the MIT License. See the LICENSE file for more details.
 */

#include <stdint.h>

uint32_t gvg_combine_colors(uint32_t src_color, uint32_t dst_color);