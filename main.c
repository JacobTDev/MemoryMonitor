//  main.c
//  MemoryMonitor

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libproc.h>
#include <sys/proc.h>

#define PROCESS_NAME_MAX 256
#define PROCESS_BUFFER_MAX 1024

struct Process {
    struct proc_taskinfo    t_info;
    uint64_t                real_mem_usage;
    pid_t                   pid;
    char                    name[PROCESS_NAME_MAX];
};

void get_process_info(struct Process *process);
void print_process_info(struct Process *process);

int main(void)
{
    uint32_t pid_buffer[PROCESS_BUFFER_MAX] = {0};
    
    for (;;) {
        // setting process count to 1 from the beginning since
        // we don't count kernel_task in the loop.
        size_t pc = 1;
        proc_listallpids(pid_buffer, sizeof(pid_buffer));
        
        for (size_t i = 0; i < PROCESS_BUFFER_MAX; i++) {
            // Check that it's not kernal_task's PID
            // (not allowed to get kernel_task process info)
            if (pid_buffer[i] != 0) {
                pc++;
                struct Process process;
                process.pid = pid_buffer[i];
                get_process_info(&process);
                process.real_mem_usage = process.t_info.pti_resident_size;
                proc_name(process.pid, process.name, sizeof(process.name));
                print_process_info(&process);
            }
        }
        
        printf("Processes running: %zu\n", pc);
        sleep(2);
    }
    
    return 0;
}

void get_process_info(struct Process *process)
{
    int err = proc_pidinfo(process->pid, PROC_PIDTASKINFO, 0, &process->t_info, sizeof(struct proc_taskinfo));
    if (err != sizeof(struct proc_taskinfo)) {
        printf("Failed to get process information for process: %d.\n", process->pid);
    }
}

void print_process_info(struct Process *process)
{
    if (strlen(process->name) > 0 && strcmp(process->name, "(null)")) {
        printf("%s: %d | Memory Usage: ", process->name, process->pid);
        
        if (process->real_mem_usage <= 500000) {
            printf("%.1fKB\n", (float)process->real_mem_usage / 1000);
        } else {
            printf("%.1fMB\n", (float)process->real_mem_usage / 1000000);
        }
    }
}
