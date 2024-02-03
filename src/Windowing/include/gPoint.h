//  gPoint.h - System - GSP
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
//  gPoint.h contains the gPoint type

#ifndef GPOINT_H
#define GPOINT_H

#include <string>

struct gPoint {
    float x;
    float y;
};

std::string ToString(gPoint point);
std::string ToDescription(gPoint point);


#endif // GPOINT_H