#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "setting.h"

int cmp(const void *a, const void *b) {return ((Process*)a)->rt > ((Process*)b)->rt;}

int main()
{
	char pol[7];
	const char* poli[] = {"FIFO", "RR", "SJF", "PSJF"};
	int N, opt = 0;
	scanf("%s%d", pol, &N);

	Process* processes;
	processes = (Process*)malloc(N*sizeof(Process));
	for(int i = 0; i < N; i++) scanf("%s%d%d", processes[i].name, &processes[i].rt, &processes[i].et);
	for(; opt < 4; opt++) if(strcmp(pol, poli[opt]) == 0) break;
	qsort(processes, N, sizeof(Process), cmp);
	scheduling(opt, processes, N);
	return 0;
}


void unit() {volatile unsigned long i;for(i = 0;i < 1000000UL;i++);}   //unit time

//Process to be defined. name, rt, et, pid
//scheduling function to be designed.
//scheduling(opt, processes, N);
