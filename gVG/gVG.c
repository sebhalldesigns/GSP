#include <gVG/gVG.h>

#include <stdio.h> // for printf etc
#include <stdlib.h> // for malloc etc


#ifdef __linux__ 
    #include <unistd.h>
    #include <pthread.h>  // for threading

    long get_num_processors() {
        long num = sysconf(_SC_NPROCESSORS_ONLN);
        printf("found %lld processors\n", num);
        return 4;
    }

    #define gvg_thread_t pthread_t
#endif

#ifdef WIN32
    #include <windows.h>

    long get_num_processors() {
        return 8;
    }

    #define gvg_thread_t HANDLE

#endif

struct threaded_fill_args {
    struct gvg_buffer_t buffer;
    size_t start_index; 
    size_t end_index; 
    uint32_t pixel_color;
};

struct thread_state {
    gvg_thread_t thread;
    struct threaded_fill_args* args;
};

static size_t g_num_processors = 0;
static struct thread_state* g_threads = NULL;

/**
 * @brief Tries to allocate a new pixel buffer, filled with white.
 * @param width The width of the buffer to allocate in pixels.
 * @param height The height of the buffer to allocate in pixels.
 * @return The allocated buffer with fields filled in. 
 * If the data section is NULL, the buffer failed to allocate.
 */
struct gvg_buffer_t gvg_buffer_alloc(size_t width, size_t height) {

    if (g_num_processors == 0) {
        g_num_processors = get_num_processors();
        g_threads = (gvg_thread_t*)malloc(g_num_processors*sizeof(struct thread_state));
    }

    //printf("SIZE %llu\n", width*height*sizeof(uint32_t));

    struct gvg_buffer_t temp_buffer;
    temp_buffer.width = 0;
    temp_buffer.height = 0;
    temp_buffer.data = NULL;

    if (width > SIZE_MAX / height) {
        printf("GVG_WARNING: ignoring buffer allocation request because the size would be too big\n");
        return temp_buffer;
    }
    
    temp_buffer.data = (uint32_t*)malloc(width*height*sizeof(uint32_t));

    if (temp_buffer.data != NULL) {
        temp_buffer.width = width;
        temp_buffer.height = height;
    } else {
        printf("GVG_ERROR: buffer allocation failed.\n");
        return temp_buffer;
    }

    return temp_buffer;
}

/**
 * @brief Tries to resize the given buffer.
 * @param buffer The buffer to try and resize.
 * @param width The desired width in pixels.
 * @param height The desired height in pixels.
 * @return The resized buffer with fields filled in. 
 * If the data section is NULL, the buffer failed to allocate.
 */
struct gvg_buffer_t gvg_buffer_resize(struct gvg_buffer_t buffer, size_t width, size_t height) {

    struct gvg_buffer_t temp_buffer;
    temp_buffer.width = 0;
    temp_buffer.height = 0;
    temp_buffer.data = NULL;

    if (width > SIZE_MAX / height) {
        printf("GVG_WARNING: ignoring buffer resize request because the size would be too big\n");
        return buffer;
    }

    temp_buffer.data = (uint32_t*)realloc(buffer.data, width*height*sizeof(uint32_t));

    if (temp_buffer.data != NULL) {
        temp_buffer.width = width;
        temp_buffer.height = height;
    } else {
        printf("GVG_ERROR: buffer reallocation failed.\n");
        return buffer;
    }

    return temp_buffer;
}

/**
 * @brief Frees a buffer.
 * @param buffer The buffer to free.
 */
void gvg_buffer_free(struct gvg_buffer_t buffer) {
    free(buffer.data);
    buffer.width = 0;
    buffer.height = 0;
}



void* threaded_fill(struct threaded_fill_args* args) {
    size_t i = args->start_index;
    size_t limit = args->buffer.width * args->buffer.height;
    while ( i < args->end_index && i < limit) {
        args->buffer.data[i] = args->pixel_color;
        i++;
    }
  
    return NULL;
}
/**
 * @brief Fills a buffer.
 * @param buffer The buffer to fill.
 * @param color The color to fill it with.
 */
void gvg_buffer_fill(struct gvg_buffer_t buffer, struct gvg_color_t color) {

    if (buffer.data == NULL) {
        printf("GVG_WARNING: ignoring buffer fill request because the buffer is invalid\n");
        return;
    }



    uint32_t pixel_color = ((uint8_t)(color.alpha * 255) << 24) 
    | ((uint8_t)(color.blue * 255) << 16) 
    | ((uint8_t)(color.green * 255) << 8) 
    | (uint8_t)(color.red * 255);

    size_t pixels_per_processor = (buffer.width * buffer.height)/g_num_processors;

   // printf("GVG_WARNING: ignoring buffer fill request because the buffer is invalid\n");


    for (size_t i = 0; i < g_num_processors; i++) {

        g_threads[i].args = malloc(sizeof(struct threaded_fill_args));
        if (g_threads[i].args != NULL) {
            g_threads[i].args->buffer = buffer;
            g_threads[i].args->start_index = g_num_processors*i;
            g_threads[i].args->end_index = pixels_per_processor*i + pixels_per_processor;
            g_threads[i].args->pixel_color = pixel_color;
            #ifdef __linux__
                pthread_create(&(g_threads[i].thread), NULL, threaded_fill, (void*)g_threads[i].args);
            #endif
            #ifdef WIN32
                g_threads[i].thread = CreateThread(NULL, 0,  (LPTHREAD_START_ROUTINE)threaded_fill, (LPVOID)g_threads[i].args, 0, NULL);
            #endif
            
            //pthread_join(g_threads[i].thread, NULL);
        }
    }
    
    for (size_t i = 0; i < g_num_processors; i++) { 
        #ifdef __linux__
        pthread_join(g_threads[i].thread, NULL);
        #endif
        #ifdef WIN32
        if (g_threads[i].thread != NULL){
           WaitForSingleObject(g_threads[i].thread,INFINITE);
            
            CloseHandle(g_threads[i].thread);
        }
        #endif
    }
    

    /*for (size_t i = 0; i < buffer.width * buffer.height; i++) {
        buffer.data[i] = pixel_color;
    }*/

    //printf("FILLING %x\n", pixel_color);

}