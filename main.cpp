#include "edge.h"
#include "scheduler.h"
#include <stdio.h>

int main()
{
	FILE *fin;
	fin = fopen("scheduler.in", "r");
	if (fin == NULL)
	{
		printf("File not found\n");
		exit(0);
	}

	Scheduler scheduler(fin);

	printf("time %d\n", scheduler.calculateMinTimeScheduling());
}
