#include "edge.h"
#include "scheduler.h"
#include <stdio.h>

int main()
{
	int rc = 0;
	FILE *fin;
	fin = fopen("scheduler.in", "r");
	if (fin == NULL)
	{
		printf("File not found\n");
		exit(0);
	}

	Scheduler scheduler{fin};

	rc  = scheduler.readFile();

	if (rc == 0)
	{
		int time = scheduler.calculateMinTimeScheduling();
		printf("time %d\n", time);

	}

	return rc;
}
