#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define GET_TIME 333
#define PRINTKK 334

typedef struct process
{
	char name[50];
	int rt, et;
	pid_t pid;
} Process;

void unit();
int get_next_id(int opt, Process* processes, int N, int *id, int *cur_t, int* last_t);
int create(Process pro);
void scheduling(int opt, Process *processes, int N);
