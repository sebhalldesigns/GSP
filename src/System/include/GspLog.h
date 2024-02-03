//  GspLog.h - System - GSP
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
//  GspLog.h contains the top-level functionality
//  for logging with GSP;

#ifndef GSPLOG_H
#define GSPLOG_H

#include "GspFunctionTypes.h"

gResult gLogInfo(const char* message);
gResult gLogWarning(const char* message);
gResult gLogError(const char* message);
gResult gLogFailure(const char* message);

#endif // GSPLOG_H