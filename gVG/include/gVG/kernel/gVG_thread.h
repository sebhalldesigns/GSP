/**
 * @file gVG_thread.h
 * @brief This file contains the gVG thread interface.
 * It is not intended for direct interaction by those using gVG. Instead simply import gVG.h.
 * This file is implemented separately for Windows and POSIX systems.
 * 
 * @date 2024/03/17
 * @author Seb Hall
 * @copyright Copyright (c) 2024
 * OpenGSP is distributed under the MIT License. See the LICENSE file for more details.
 */

#include <stdint.h> // for uintptr_t

#ifdef WIN32
    
#else // POSIX 
    #include <gVG/kernel/posix/gVG_job_posix.h>
#endif // WIN32

/**
 * @brief thread main function
 */
void* gvg_thread_main(void* args);

/**
 * @brief thread main function
 * @param job_id The job ID.
 * @return The job status
 */
gvg_job_status_t gvg_thread_get_job_status(uintptr_t job_id);

/**
 * @brief thread main function
 * @param job A pointer to the job to be submitted. Do not try and interact with this
 * memory after submitting. Instead store the ID for future use.
 * @return The job ID. If this is zero, then the job failed to add to the queue.
 */
uintptr_t gvg_thread_submit_job(gvg_job_t* job);

/**
 * @brief thread main function
 * @param job_id The job ID to retrieve.
 * @return The job pointer that is retrieved. If it is nonzero (i.e not NULL), the job has
 * been removed from the queue and can safely be interacted with. If it returns NULL, the job
 * has either not been completed, or is an invalid job.
 */
gvg_job_t* gvg_thread_retrieve_job(uintptr_t job_id);






