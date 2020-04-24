#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <string.h>

#include "scheduler.h"
#include "process_control.h"

int scheduler_RR(Process *proc, int N_procs){

	// pid_t chpids[N_procs] = {0};

	int N_fin = 0; //number of finished processes
	int cur_t = 0; //current time

	while(1){

		int nt = 0; // number of processes not allowed to be executed
		int next_ex_t = INT_MAX; //the closest ready time from current time (used when nt == N_procs)

		for(int i=0; i<N_procs; i++){

			if( cur_t < proc[i].ready_time ){
				if( proc[i].ready_time < next_ex_t ) next_ex_t = proc[i].ready_time;
				nt ++;
				continue;
			}else if( proc[i].exec_time <= 0 ){
				nt ++;
				continue;
			} 

			if( proc[i].pid > 0 ){
				proc_resume( proc[i].pid );
			}else{ // if process hasn't been created
				proc[i].pid = proc_create( proc[i] );
				proc_resume( proc[i].pid );
			}

			// run an RR round
			int kt = RR_SLICE; //time quantum for RR
			while( proc[i].exec_time > 0 && kt > 0){
				write(proc[i].pipe_fd[1], "run", strlen("run")); // tell process to run 1 time unit
				TIME_UNIT(); // run 1 time unit itself
				kt --;
				proc[i].exec_time --;
				cur_t ++;
			}

			// if process finished
			if(proc[i].exec_time <= 0){
				int re_status;
				waitpid(proc[i].pid, &re_status, 0);
				if( !(WIFEXITED(re_status)) ){
					perror("error: child process terminated inappropriately");
					return 1;
				}
				N_fin ++;
			}else{
				proc_kickout( proc[i].pid );				
			}

		}

		if( N_fin >= N_procs ) break;

		if( nt >= N_procs){ // run itself when not finished and no process can be executed
			while( cur_t < next_ex_t ){ //until at least a process is ready
				TIME_UNIT();
				cur_t ++;
			}
		}
	}

	return 0;
}
