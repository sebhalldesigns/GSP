/**
 * @file gVG_job_posix.h
 * @brief This file contains definitions for the POSIX job interface.
 * 
 * @date 2024/03/17
 * @author Seb Hall
 * @copyright Copyright (c) 2024
 * OpenGSP is distributed under the MIT License. See the LICENSE file for more details.
 */

#include <stdint.h> // for uintptr_t

/**
 * @brief status of a job submitted to the thread pool
 */
typedef enum {
    PENDING,
    ACTIVE,
    COMPLETE
} gvg_job_status_t;

/**
 * @brief status of a job submitted to the thread pool
 */
typedef struct {
    gvg_job_status_t status;
    char message[256];
} gvg_job_t;





