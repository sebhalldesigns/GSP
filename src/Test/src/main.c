#include "gvg.h"

#include <stdio.h>

int main() {

    char* msg = send_msg();

    printf(msg);
    printf("\n");

    return 0;
}