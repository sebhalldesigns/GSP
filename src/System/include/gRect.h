//  gRect.h - System - GSP
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
//  gRect.h contains the gRect type

#ifndef GRECT_H
#define GRECT_H

#include <string>

struct gRect {
    float x;
    float y;
    float width;
    float height;
};

std::string ToString(gRect point);
std::string ToDescription(gRect point);


#endif // GRECT_H