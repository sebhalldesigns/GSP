//  gSize.h - System - GSP
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
//  gSize.h contains the gSize type

#ifndef GSIZE_H
#define GSIZE_H

#include <string>

struct gSize {
    float width;
    float height;
};

std::string ToString(gSize size);
std::string ToDescription(gSize size);


#endif // GSIZE_H