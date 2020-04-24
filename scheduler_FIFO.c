#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "scheduler.h"
#include "process_control.h"

int scheduler_FIFO(Process *proc, int N_procs){

	int cur = -1;
	int total_time=0;

	while(1){

		cur += 1;
		if( cur >= N_procs ) break;

		while( proc[cur].ready_time > total_time ){
			TIME_UNIT();
			// wait 1 time unit until proc is ready
			total_time += 1;
		}
		pid_t chpid = proc_create(proc[cur]);
		proc_resume( chpid );
		while( proc[cur].exec_time > 0 ){
			// tell process to run 1 time unit
			write(proc[cur].pipe_fd[1], "run", strlen("run"));
			// run 1 time unit itself
			TIME_UNIT();
			total_time += 1;
			proc[cur].exec_time -= 1;
		}

		// wait child process
		int re_status;
		waitpid(chpid, &re_status, 0);
		if( !(WIFEXITED(re_status)) ){
			perror("error: child process terminated inappropriately");
			return 1;
		}

	}

	return 0;
}
