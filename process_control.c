#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "process_control.h"
#include "scheduler.h"

#define SYS_PRINT_STR 333

typedef struct sched_param Sched_pm;
typedef struct timespec Time_sp;

void inline TIME_UNIT(void){
    volatile unsigned long i;
    for(i = 0; i < 1000000UL; i++);
    return;
}

int assign_core(pid_t pid, int core){
    cpu_set_t cpu_mask;

    if (core > sizeof(cpu_mask)){
        fprintf(stderr, "Invalid Core No.: %d\n", core);
        return -1;
    }

    CPU_ZERO(&cpu_mask);
    CPU_SET(core, &cpu_mask);

    if ( sched_setaffinity(pid, sizeof(cpu_mask), &cpu_mask) != 0 ){
        perror("error: sched_setaffinity");
        exit(-1);
    }

    return 0;
}

pid_t proc_create(Process chld){
    pid_t chpid = fork();
    if ( chpid < 0 ){
        perror("error: fork");
        exit(2);
    }

    if ( chpid == 0 ){ // emulate child processes
        close( chld.pipe_fd[1] );
        int init_exec_time = chld.exec_time;

        Time_sp start, end;
        char dmesg[256] = "";

        while( chld.exec_time > 0 ){
            // synchronize with scheduler
            char buf[8];
            read(chld.pipe_fd[0], buf, strlen("run"));
            
            if( chld.exec_time == init_exec_time ){
                if( clock_gettime(CLOCK_REALTIME, &start) == -1 ){
                    perror("error: clock_gettime");
                    exit(3);
                }
                printf("%s %d\n", chld.name, getpid());
            }
            
            TIME_UNIT();
            chld.exec_time--;
#ifdef PRINT_CHLD
            fprintf(stderr, "%s, rounds left %d\n", chld.name, chld.exec_time);
#endif
        }
        if( clock_gettime(CLOCK_REALTIME, &end) == -1 ){
            perror("error: clock_gettime");
            exit(3);
        }

        sprintf(dmesg, "[Project1] %d %09lu.%09lu %09lu.%09lu\n",
                getpid(), start.tv_sec, start.tv_nsec, end.tv_sec, end.tv_nsec);
#ifdef PRINT_CHLD
	printf("len of dmesg: %d\n", (int)strlen(dmesg));
#endif
        syscall(SYS_PRINT_STR, dmesg, strlen(dmesg)+1);

        exit(0);
    }
    
    proc_kickout(chpid);
    assign_core(chpid, CHILD_CORE);
    close( chld.pipe_fd[0] );

    return chpid;
}

int proc_kickout(pid_t pid){
    Sched_pm sp;
    sp.sched_priority = 0;

    if ( sched_setscheduler(pid, SCHED_IDLE, &sp) < 0 ){
        perror("error: sched_setscheduler");
        return -1;
    }

    return 0;
}

int proc_resume(pid_t pid){
    Sched_pm sp;
    sp.sched_priority = 0;

    if ( sched_setscheduler(pid, SCHED_OTHER, &sp) < 0 ){
        perror("error: sched_setscheduler");
        return -1;
    }

    return 0;
}
