#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#define PROCESS_NAME_MAX 32

typedef enum{
    PROCESS_READY, PROCESS_RUNNING,
    PROCESS_BLOCKED, PROCESS_TERMINATED
} process_state_t;

typedef struct process{
    uint32_t pid;
    char name[PROCESS_NAME_MAX];
    process_state_t state;
    struct process* next;
} process_t;

void process_init(void);

process_t* process_create(const char* name);

void process_destroy(process_t* process);

process_t* process_find(uint32_t pid);

void process_list_all(void);

process_t* process_get_current(void);

process_t* get_current(void);

process_t* process_get_next_ready(void);

process_t* scheduler_next(void);

void scheduler_run_next(void);

void process_set_current(process_t* process);

void process_set_state(process_t* process, process_state_t state);

#endif
