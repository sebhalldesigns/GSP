use super::painting::*;

#[repr(C)]
pub struct VgPathInstruction {
    pub instruction: u8,
    pub param_count: usize,
    pub params: *mut f64
}

#[repr(C)]
pub struct VgPath {
    pub instruction_count: usize,
    pub instructions: *mut VgPathInstruction,
    pub fill_properties: VgFillProperties,
    pub stroke_properties: VgStrokeProperties
}
