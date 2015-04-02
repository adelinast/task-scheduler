#include "edge.h"
#include "scheduler.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	int rc = 0;
	FILE *fin = NULL;
	FILE *fout = NULL;
	char *filename = NULL;

	if (argc != 2)
	{
		printf("Usage %s <filename>\n", argv[0]);
		exit(1);
	}
	else
	{
		filename = argv[1];
		if (filename == NULL)
		{
			printf("Error: Filename is NULL");
			exit(1);
		}
	}
	
	fin = fopen(argv[1], "r");
	if (fin == NULL)
	{
		printf("Error: File not found\n");
		exit(1);
	}

	Scheduler scheduler(fin);

	rc  = scheduler.readFile();
	int time = -1;
	if (rc == 0)
	{
		time = scheduler.calculateMinTimeScheduling();
		printf("time %d\n", time);
	}

	if (time != -1)
	{
		fout = fopen("test.out", "w");
		if (fout == NULL)
		{
			printf("Error: File not found\n");
			exit(1);
		}

		fprintf(fout, "%d", time);
	}

	fclose(fin);
	fclose(fout);

	return rc;
}
