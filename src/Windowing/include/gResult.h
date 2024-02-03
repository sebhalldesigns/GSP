//  gResult.h - System - GSP
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
//  gResult.h contains the gResult type

#ifndef GRESULT_H
#define GRESULT_H

#include <string>

enum gResult {
    SUCCESS,
    WARNING,
    ERROR,
    FAILURE
};

std::string ToString(gResult result);
std::string ToDescription(gResult result);


#endif // GRESULT_H