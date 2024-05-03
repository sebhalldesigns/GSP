#ifndef GLOG_H
#define GLOG_H

typedef enum {
    INFO,
    WARNING,
    ERROR,
    FAILURE
} GLogLevel;

void GLog(GLogLevel level, const char* message, ...);

#endif // GLOG_H