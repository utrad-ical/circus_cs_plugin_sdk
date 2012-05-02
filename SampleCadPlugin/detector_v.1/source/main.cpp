#pragma warning(disable:4996) 

#include<stdio.h>

#include "VOL.h"
#include "LibCircusCS.h"

#include "detector.h"
#include "results.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{
	// Check the number of arguments
	if(argc != 4)
	{
		fprintf(stderr, "[Usage] detector_v.1.exe jobRootPath seriesNum coreNum\n");	
		return -1;
	}

	char jobRootPath[1024];

	// Set job root path
	strcpy(jobRootPath, argv[1]);

	// Get the number of cores
	int coreNum = atoi(argv[3]);

	// CAD detector
	VOL_INTBOX3D* cropBox = VOL_NewIntBox3D(VOL_NewIntVector3D(0,0,0), VOL_NewIntSize3D(0,0,0));
	VOL_MATRIX* detectResult = detectorMain(jobRootPath, cropBox, coreNum);

	// Create result files
	createResultFiles(jobRootPath, detectResult, cropBox);

	VOL_DeleteMatrix(detectResult);

	return 0;
}
