use libc::c_char;

pub use crate::basic_types::*;

#[repr(C)]
pub enum VgShapeType {
    RECT,
    CIRCLE,
    ELLIPSE,
    LINE,
    POLYLINE,
    POLYGON
}

#[repr(C)]
pub enum VgPaintType {
    NONE,
    COLOR,
    NAMED,
    FUNC,
    INHERIT
}

#[repr(C)]
pub enum VgFillRule {
    NONZERO,
    EVENODD,
    INHERIT
}

#[repr(C)]
pub enum VgStrokeCap {
    BUTT,
    ROUND,
    SQUARE,
    INHERIT
}

#[repr(C)]
pub enum VgStrokeJoin {
    MITER,
    ROUND,
    BEVEL,
    INHERIT
}

#[repr(C)]
pub struct VgFillProperties {
    pub paint_type: VgPaintType,

    pub paint_color: VgColor,
    pub paint_func: *mut c_char,
    pub paint_named: *mut c_char,

    pub opacity: f64,
    pub rule: VgFillRule
}

#[repr(C)]
pub struct VgStrokeProperties {
    pub paint_type: VgPaintType,

    pub paint_color: VgColor,
    pub paint_func: *mut c_char,
    pub paint_named: *mut c_char,

    pub opacity: f64,
    pub width: f64,

    pub dash_pulse_width: f64,
    pub line_cap: VgStrokeCap,
    pub line_join: VgStrokeJoin, 
    
    // fairly obscure parameter. should be specified in render settings
    //pub line_join_miter_limit: f64
}




