//  gColor.h - System - GSP
//  First created by Seb Hall on 03/Feb/2024
//
//  GSP is a cross-platform software framework
//  that provides abstraction layers, libraries,
//  packaging and build functionality. GSP is
//  designed around a modular architecture that
//  promotes simplicity and encapsulation, and
//  we hope that you will do amazing things with
//  it!
//
//  gColor.h contains the gColor type

#ifndef GCOLOR_H
#define GCOLOR_H

#include <string>

struct gColor {
    float r;
    float g;
    float b;
    float a;
};

std::string ToString(gColor point);
std::string ToDescription(gColor point);


#endif // GCOLOR_H