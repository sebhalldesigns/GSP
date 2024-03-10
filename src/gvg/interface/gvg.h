#ifndef GVG_H
#define GVG_H

#include <stddef.h>
#include <stdint.h>

typedef struct VgColor {
    double red;
    double green;
    double blue;
    double alpha;
};

typedef enum VgPaintType {
    NONE,
    COLOR,
    NAMED,
    FUNC,
    INHERIT
};

typedef enum VgFillRule {
    NONZERO,
    EVENODD,
    INHERIT
};

typedef enum VgStrokeCap {
    BUTT,
    ROUND,
    SQUARE,
    INHERIT
};

typedef enum VgStrokeJoin {
    MITER,
    ROUND,
    BEVEL,
    INHERIT
};

typedef struct VgFillProperties {
    enum VgPaintType paint_type;

    struct VgColor paint_color;
    char* paint_func;
    char* paint_named;

    double opacity;
    enum VgFillRule rule;
};

typedef struct VgStrokeProperties {
    enum VgPaintType paint_type;

    struct VgColor paint_color;
    char* paint_func;
    char* paint_named;

    double opacity;
    double width;

    double dash_pulse_width;
    enum VgStrokeCap line_cap;
    enum VgStrokeJoin line_join;
};

typedef enum VgShapeType {
    RECT,
    CIRCLE,
    ELLIPSE,
    LINE,
    POLYLINE,
    POLYGON
};

typedef struct VgShape {
    enum VgShapeType shape_type;
    
    size_t param_count;
    double* params;

    struct VgFillProperties fill_properties; 
    struct VgStrokeProperties stroke_properties;
};

typedef struct VgPathInstruction {
    uint8_t instruction;
    size_t param_count;
    double* params;
};

typedef struct VgPath {
    size_t instruction_count;
    struct VgPathInstruction* instructions;

    struct VgFillProperties fill_properties; 
    struct VgStrokeProperties stroke_properties;
};

typedef struct VgPixelBuffer {
    size_t width;
    size_t height;
    uint32_t* data;
};

struct VgPixelBuffer vgpixelbuffer_alloc(size_t width, size_t height);

typedef void* VgEngineState;

// generate state
VgEngineState vg_init();

// reset state
void vg_reset(VgEngineState state);

void vg_render(VgEngineState state, struct VgPixelBuffer buffer);

void vg_add_shape(VgEngineState state, struct VgShape shape);

void vg_add_path(VgEngineState state, struct VgPath path);

void vg_final(VgEngineState state);



#endif // GVG_H