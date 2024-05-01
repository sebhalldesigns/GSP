#include <gVG/kernel/gVG_thread.h>

#include <unistd.h> // for sleep
#include <stdbool.h> // for true
#include <stdio.h> // for printf
#include <stddef.h> // for size_t



/**
 * @brief thread main function
 */
void* gvg_thread_main(void* args) {
    size_t id = *(size_t*)args;
    printf("  Thread %lu started\n", id);

    while (true) {
        sleep(1);
        printf("  Thread %lu poll\n", id);
    }

    return 0;
}

/**
 * @brief thread main function
 * @param job_id The job ID.
 * @return The job status
 */
gvg_job_status_t gvg_thread_get_job_status(uintptr_t job_id) {
    return PENDING;
}

/**
 * @brief thread main function
 * @param job A pointer to the job to be submitted. Do not try and interact with this
 * memory after submitting. Instead store the ID for future use.
 * @return The job ID. If this is zero, then the job failed to add to the queue.
 */
uintptr_t gvg_thread_submit_job(gvg_job_t* job) {
    return 0;
}

/**
 * @brief thread main function
 * @param job_id The job ID to retrieve.
 * @return The job pointer that is retrieved. If it is nonzero (i.e not NULL), the job has
 * been removed from the queue and can safely be interacted with. If it returns NULL, the job
 * has either not been completed, or is an invalid job.
 */
gvg_job_t* gvg_thread_retrieve_job(uintptr_t job_id) {
    return NULL;
}








