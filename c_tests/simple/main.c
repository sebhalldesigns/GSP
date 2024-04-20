#include <stdbool.h>
#include "gsp_system.h"
#include <stdio.h>

void run_callback() {
    printf("CALLBACK FOROM C\n");
}

int main() {

    gsp_init(run_callback);

    return gsp_run();
}