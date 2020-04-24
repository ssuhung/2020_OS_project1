#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>

#include "process_control.h"


int find_shortest(Process *proc, int N_procs, int time){
	int shortest = -1, excute_time = INT_MAX;

	for (int i = 0; i < N_procs; i++){
		if (proc[i].ready_time <= time && proc[i].exec_time && proc[i].exec_time < excute_time){
			excute_time = proc[i].exec_time;
			shortest = i;
		}
	}

	return shortest;
}

int scheduler_SJF(Process *proc, int N_procs){
	int finish = 0;
	int time = 0;

#ifdef PRINT_LOG
	FILE *fp = fopen("./scheduler_log/SJF_5.out", "wb");
	char mesg[256] = "";
#endif

	while (finish < N_procs){
		int target = find_shortest(proc, N_procs, time);
		
		if (target != -1){
#ifdef PRINT_LOG
			sprintf(mesg, "process %s, start at %d\n", proc[target].name, time);
			fprintf(fp, "%s", mesg);
			fflush(fp);
#endif

			pid_t chpid = proc_create(proc[target]);
			proc_resume( chpid );
			proc[target].pid = chpid;

			while (proc[target].exec_time > 0){
				// tell child process to run 1 time unit
				char tmp[5] = "run";
				write(proc[target].pipe_fd[1], tmp, strlen(tmp));
				
				TIME_UNIT();
				time++;
				proc[target].exec_time--;
			}

			finish++;			
			// wait child process
			int _return;
			waitpid(proc[target].pid, &_return, 0);
			
			if (WIFEXITED(_return) != 1){
				fprintf(stderr, "error: child process terminated inappropriately");
				return 1;
			}
#ifdef PRINT_LOG
			sprintf(mesg, "process %s, end at %d\n", proc[target].name, time);
			fprintf(fp, "%s", mesg);
			fflush(fp);
#endif
		}

		else{
			TIME_UNIT();
			time++;
		}
	}

#ifdef PRINT_LOG
	fclose(fp);
#endif

	return 0;
}
