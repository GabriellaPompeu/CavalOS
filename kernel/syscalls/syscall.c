#include "syscall.h"
#include "../process/process.h"
#include "../filesystem/filesystem.h"
#include "../terminal/terminal.h"

void syscall_init(void){

}

int32_t syscall_dispatch(uint32_t number, uint32_t arg1, uint32_t arg2, uint32_t arg3){

    (void)arg1;
    (void)arg2;
    (void)arg3;

    switch (number){
        case SYS_WRITE: terminal_write_string((const char*)arg1);
            return 0;
        case SYS_PS: process_list_all();
            return 0;
        case SYS_LS: filesystem_list_files(terminal_get_filesystem());
            return 0;
        case SYS_GETPID:{
            process_t* current = process_get_current();
            if(current == NULL) return -1;
            return current -> pid;
        }
        case SYS_EXIT:{
            process_t* current = process_get_current();
            if(current == NULL) return -1;
            process_set_state(current, PROCESS_TERMINATED);
            return 0;
        }
        default: return -1;
    }
}

int32_t syscall(uint32_t number, uint32_t arg1, uint32_t arg2, uint32_t arg3){
    return syscall_dispatch(number, arg1, arg2, arg3);
}
