#include <gVG/pipeline/common/gvg_common.h>

uint32_t gvg_combine_colors(uint32_t src_color, uint32_t dst_color) {

    uint8_t src_bias = (src_color & 0xFF000000) >> 24;
    uint8_t dst_bias = 0xFF - src_bias;
    uint8_t alpha_bias = (dst_color & 0xFF000000) >> 24;

    uint8_t blended_blue = ((src_color & 0x000000FF) * src_bias + (dst_color & 0x000000FF) * dst_bias) / 0xFF;
    uint8_t blended_green = (((src_color & 0x0000FF00) >> 8) * src_bias + ((dst_color & 0x0000FF00) >> 8) * dst_bias) / 0xFF;
    uint8_t blended_red = (((src_color & 0x00FF0000) >> 16) * src_bias + ((dst_color & 0x00FF0000) >> 16)  * dst_bias) / 0xFF;

    uint8_t blended_alpha = alpha_bias + src_bias*(0xFF-alpha_bias);

    return (blended_alpha << 24) | (blended_red << 16) | (blended_green << 8) | blended_blue;
}