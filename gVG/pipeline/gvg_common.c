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

/*

  

    uint8_t src_alpha = (src_color & 0xFF000000) >> 24;
    uint8_t src_red = (src_color & 0x00FF0000) >> 16;
    uint8_t src_green = (src_color & 0x0000FF00) >> 8;
    uint8_t src_blue = (src_color & 0x000000FF);

    uint8_t dst_alpha = (dst_color & 0xFF000000) >> 24;
    uint8_t dst_red = (dst_color & 0x00FF0000) >> 16;
    uint8_t dst_green = (dst_color & 0x0000FF00) >> 8;
    uint8_t dst_blue = (dst_color & 0x000000FF);

    // Invert the source alpha for smoother blending
    uint8_t inv_src_alpha = 255 - src_alpha;





  // Blend the red, green, and blue components using alpha blending
  uint8_t blended_red = (src_red * src_alpha + dst_red * inv_src_alpha) / 255;
  uint8_t blended_green = (src_green * src_alpha + dst_green * inv_src_alpha) / 255;
  uint8_t blended_blue = (src_blue * src_alpha + dst_blue * inv_src_alpha) / 255;


    uint8_t blended_alpha = dst_alpha + src_alpha*(255-dst_alpha);

  // Pack the blended components back into a uint32_t color
  return (blended_alpha << 24) | (blended_red << 16) | (blended_green << 8) | blended_blue;*/