#include <gVG/kernel/gVG_kernel.h>
#include <gVG/kernel/gVG_thread.h>

#include <unistd.h> // for _SC_NPROCESSORS_ONLN
#include <stdlib.h> // for malloc
#include <stdio.h> // for printf
#include <pthread.h>  // for threading


static size_t g_num_processors = 0;
static pthread_t* g_thread_pool = NULL;

/**
 * @brief Initialises the kernel
 */
void gvg_kernel_init() {

    printf("\n  Welcome to gVG - the OpenGSP Vector Graphics library\n");

    long num_processors = sysconf(_SC_NPROCESSORS_ONLN);
    printf("  Found %ld logical processors\n", num_processors);
    g_num_processors = num_processors;
    
    // WARNING: if num_processors * sizeof(size_t) > SIZE_MAX then malloc will fail
    printf("  Initializing thread pool...\n");
    g_thread_pool = (pthread_t*)malloc(g_num_processors*sizeof(size_t));
    if (g_thread_pool == NULL) {
        printf("  ERROR: failed to allocate thread pool.\n");
        return;
    }

    for (size_t thread_i = 0; thread_i < g_num_processors; thread_i++) {
        printf("Allocating thread %lu\n", thread_i);
        size_t* thread_index = (size_t*)malloc(sizeof(size_t));
        *thread_index = thread_i;
        pthread_create(&g_thread_pool[thread_i], NULL, gvg_thread_main, (void*)thread_index);
    }

}