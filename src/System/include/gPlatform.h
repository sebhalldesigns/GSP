//  gPlatform.h - System - GSP
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
//  gPlatform.h contains the top level definitions 
//  and interface for platform functionality.

#ifndef GPLATFORM_H
#define GPLATFORM_H

#include <string>

enum gPlatform {
    UNDEFINED,
    WINDOWS,
    UNIXLIKE,
    DARWIN,
    ANDROID,
    WEB
};  

std::string ToString(gPlatform platform);
std::string ToDescription(gPlatform platform);


#endif // GPLATFORM_H