#define _CRT_SECURE_NO_DEPRECATE 

#include<stdio.h>

#include "VOL.h"
#include "LibCircusCS.h"

#include "measurement.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RESULT_FILE_NAME  "results.txt"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{
	// Check the number of arguments
	if(argc != 4)
	{
		fprintf(stderr, "[Usage] measurement_v.1.exe jobRootPath seriesNum coreNum\n");	
		return -1;
	}

	char jobRootPath[1024];

	// Set job root path
	strcpy(jobRootPath, argv[1]);

	// Get the number of cores
	int coreNum = atoi(argv[3]);

	// Measuement main
	measurmentMain(jobRootPath, coreNum);

	return 0;
}
