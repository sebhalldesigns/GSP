//  gLog.h - System - GSP
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
//  gLog.h contains the top-level functionality
//  for logging with GSP;

#ifndef GLOG_H
#define GLOG_H

#include "gResult.h"
#include <string>

gResult gLogInfo(std::string message);
gResult gLogWarning(std::string message);
gResult gLogError(std::string message);
gResult gLogFailure(std::string message);

#endif // GLOG_H