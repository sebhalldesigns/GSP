
pub use crate::basic_types::*;
use crate::painting::*;

#[repr(C)]
pub struct VgRect {
    pub origin: VgPoint,  
    pub width: f64,
    pub height: f64,
    pub rx: f64,
    pub ry: f64,

    pub fill: VgFillProperties,
    pub stroke: VgStrokeProperties
}


#[repr(C)]
pub struct VgCircle {
    pub center: VgPoint,   
    pub r: f64,

    pub fill: VgFillProperties,
    pub stroke: VgStrokeProperties
}

#[repr(C)]
pub struct VgEllipse {
    pub center: VgPoint,
    pub rx: f64,
    pub ry: f64,

    pub fill: VgFillProperties,
    pub stroke: VgStrokeProperties
}

#[repr(C)]
pub struct VgLine {
    pub start: VgPoint,
    pub end: VgPoint,
    
    pub stroke: VgStrokeProperties
}

#[repr(C)]
pub struct VgPolyLine {
    pub points: Vec<VgPoint>,

    pub fill: VgFillProperties,
    pub stroke: VgStrokeProperties
}

#[repr(C)]
pub struct VgPolygon {
    pub points: Vec<VgPoint>,

    pub fill: VgFillProperties,
    pub stroke: VgStrokeProperties
}





