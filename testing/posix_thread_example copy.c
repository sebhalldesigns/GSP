#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // For sysconf

void *thread_function(void *arg) {
    long thread_id = (long)arg;
    printf("Thread %ld started.\n", thread_id);
    // Your thread logic goes here
    return NULL;
}

int main() {
    long num_processors = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t *threads = (pthread_t *)malloc(num_processors * sizeof(pthread_t));
    if (threads == NULL) {
        printf("ERROR: Memory allocation failed.\n");
        return 1;
    }

    long t;
    int rc;

    // Create threads
    for (t = 0; t < num_processors; t++) {
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, thread_function, (void *)t);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            return 1;
        }
    }

    // Join threads
    for (t = 0; t < num_processors; t++) {
        pthread_join(threads[t], NULL);
    }

    free(threads);
    return 0;
}
