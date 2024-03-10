

#[repr(C)]
pub struct VgPoint {
    pub x: f64,
    pub y: f64
}

#[repr(C)]
pub struct VgColor {
    pub red: f64,
    pub green: f64,
    pub blue: f64,
    pub alpha: f64
}

#[repr(C)]
pub struct VgSize {
    pub width: f64,
    pub height: f64
}
