#ifndef GGPU_H
#define GGPU_H

#include <gWin/gWin.h>

typedef enum {
    UNDEFINED,
    VULKAN,
    METAL,
    WEBGL,
    SGL
} ggpu_backend_t;


// try to initialize ggpu, return 1 if successful, 0 if unsuccessful
int ggpu_init();




int ggpu_init_for_window(gwin_handle_t window);






#endif // GGPU_H



