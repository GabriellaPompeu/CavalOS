#include "process.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../memory/heap.h"

extern void terminal_printf(const char* format, ...);

static process_t* process_list = NULL;
static process_t* current_process = NULL;
static uint32_t next_pid = 1;

void process_init(void){
    process_list = NULL;
    current_process = NULL;
    next_pid = 1;
}

process_t* process_create(const char* name){
    if(name == NULL) return NULL;

    process_t* process = kmalloc(sizeof(process_t));

    if(process == NULL) return NULL;

    process -> pid = next_pid++;

    strncpy(process -> name, name, PROCESS_NAME_MAX - 1);

    process -> name[PROCESS_NAME_MAX - 1] = '\0';
    process -> state = PROCESS_READY;
    process -> next = NULL;

    if(process_list == NULL) process_list = process;
    else{
        process_t* current = process_list;

        while(current -> next != NULL) current = current -> next;

        current -> next = process;
    }
    return process;
}

process_t* process_find(uint32_t pid){
    process_t* current = process_list;
    while(current != NULL){
        if(current -> pid == pid) return current;
        current = current -> next;
    }
    return NULL;
}

void process_list_all(void){
    process_t* current = process_list;
    terminal_printf("\nPID | NOME | ESTADO\n");
    terminal_printf("----------------\n");
    while(current != NULL){
        terminal_printf("%d | %s | ", current -> pid, current -> name);
        switch (current -> state){
            case PROCESS_READY: terminal_printf("READY\n");
                break;
            case PROCESS_RUNNING: terminal_printf("RUNNING\n");
                break;
            case PROCESS_BLOCKED: terminal_printf("BLOCKED\n");
                break;
            case PROCESS_TERMINATED: terminal_printf("TERMINATED\n");
                break;
            default: terminal_printf("UNKOWN\n");
                break;
        }

        current = current -> next;
    }
}

process_t* process_get_current(void){
    return current_process;
}

void process_destroy(process_t* process){
    if(process == NULL) return;

    if(process_list == process) process_list = process -> next;
    else{

        process_t* current = process_list;
        while(current != NULL && current -> next != process) current = current -> next;
        if(current != NULL) current -> next = process -> next;
    }

    if(current_process == process) current_process = NULL;
    kfree(process);
}

void process_set_state(process_t* process, process_state_t state){
    if(process == NULL) return;
    process -> state = state;
}
