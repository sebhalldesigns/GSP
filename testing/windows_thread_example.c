#include <stdio.h>
#include <stddef.h>
#include <Windows.h>

#include <stdio.h>
#include <windows.h>

DWORD WINAPI thread_function(LPVOID lpParam) {
    DWORD thread_id = (DWORD)lpParam;
    printf("Thread %lu started.\n", thread_id);
    // Your thread logic goes here
    return 0;
}

int main() {
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    DWORD num_processors = sys_info.dwNumberOfProcessors;

    HANDLE *threads = (HANDLE *)malloc(num_processors * sizeof(HANDLE));
    if (threads == NULL) {
        printf("ERROR: Memory allocation failed.\n");
        return 1;
    }

    DWORD t, dwThreadId;
    // Create threads
    for (t = 0; t < num_processors; t++) {
        printf("Creating thread %lu\n", t);
        threads[t] = CreateThread(NULL, 0, thread_function, (LPVOID)t, 0, &dwThreadId);
        if (threads[t] == NULL) {
            printf("ERROR: failed to create thread %lu\n", t);
            return 1;
        }
    }

    // Wait for all threads to finish
    WaitForMultipleObjects(num_processors, threads, TRUE, INFINITE);

    // Close thread handles
    for (t = 0; t < num_processors; t++) {
        CloseHandle(threads[t]);
    }

    free(threads);
    return 0;
}
