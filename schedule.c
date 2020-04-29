#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "setting.h"


void scheduling(int opt, Process *processes, int N)
{
	int cur_t = 0, id = -1, finish_n = 0, last_t = 0;
	cpu_set_t mask, mask1;
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	CPU_ZERO(&mask1);
	CPU_SET(1, &mask1);

	for(int i = 0; i < N; i++) processes[i].pid = -1;

	struct sched_param param, param1;
	param.sched_priority = sched_get_priority_max((opt == 1)? SCHED_RR :SCHED_FIFO);
	param1.sched_priority = 0;
	sched_setscheduler(getpid(), SCHED_OTHER, &param1);

	while(1)
	{
		if(id != -1 && processes[id].et == 0)
		{
			waitpid(processes[id].pid, NULL, 0);
			printf("%s %d\n", processes[id].name, processes[id].pid);
			id = -1;
			finish_n++;
			if(finish_n == N) break;
		}
		for(int i = 0; i < N; i++)
		{
			if(processes[i].rt == cur_t)
			{
				processes[i].pid = create(processes[i]);
				sched_setscheduler(processes[i].pid, SCHED_IDLE, &param);
			}
		}
		int next_id = get_next_id(opt, processes, N, &id, &cur_t, &last_t);
		if(next_id != -1 && id != next_id)
		{
			sched_setscheduler(processes[next_id].pid, SCHED_OTHER, &param);
			sched_setscheduler(processes[id].pid, SCHED_IDLE, &param1);
			last_t = cur_t;
			id = next_id;
		}

		unit();
		if(id != -1) processes[id].et--;
		cur_t++;
	}
}

pid_t create(Process pro)
{
	pid_t pid = fork();
	if(pid == 0)
	{
		int _pid = getpid();
		unsigned long start_sec, start_nsec, end_sec, end_nsec;
		char dmesg[200];
		syscall(GET_TIME,&start_sec,&start_nsec);
		for(int i = 0; i < pro.et; i++) unit();
		syscall(GET_TIME,&end_sec,&end_nsec);
		sprintf(dmesg, "[Project1] %d %lu.%09lu %lu.%09lu\n", _pid, start_sec, start_nsec, end_sec, end_nsec);
		syscall(PRINTK, dmesg);
		exit(0);
	}
	return pid;
}

int get_next_id(int opt, Process* processes, int N, int *id, int *cur_t, int* last_t)
{
	if(*id != -1 && (opt == 0 || opt == 2)) return *id;
	int tmp = -1;
	if(opt == 0)
	{
		int tmp = -1;
		for(int i = 0; i < N; i++)
		{
			if(processes[i].pid == -1 || processes[i].et == 0) continue;
			else if(tmp == -1 || processes[i].rt < processes[tmp].rt) tmp = i;
		}
	}
	else if(opt == 1)
	{
		if(*id==-1) for(int i = 0; i < N; i++) {if(processes[i].pid != -1 && processes[i].et > 0) {tmp = i; break;}}
		else if((cur_t - last_t) % 500 == 0)
		{
			tmp = (*id + 1) % N;
			while(processes[tmp].pid == -1 || processes[tmp].et == 0) tmp = (tmp + 1) % N;
		}
		else tmp = *id;
	}
	else if(opt == 2 || opt == 3) 
	{
		for(int i = 0; i < N; i++) 
		{
			if(processes[i].pid == -1 || processes[i].et == 0) continue;
			else if(tmp == -1 || processes[i].et < processes[tmp].et) tmp = i;
		}
	}
	return tmp;
}
