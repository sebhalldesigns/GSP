use super::painting::*;

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
pub struct VgShape {
    pub shape_type: VgShapeType,
    pub param_count: usize,
    pub params: *mut f64,
    pub fill_properties: VgFillProperties,
    pub stroke_properties: VgStrokeProperties
}

