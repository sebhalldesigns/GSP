#ifndef GSP_SYSTEM_H
#define GSP_SYSTEM_H

#include <stdbool.h>

typedef void (*gsp_application_launch_callback_t)();

bool gsp_init(gsp_application_launch_callback_t launch_callback);

int gsp_run();



#endif // GSP_SYSTEM_H
