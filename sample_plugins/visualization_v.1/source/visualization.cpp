#pragma warning(disable:4996)

#include <stdio.h>

#include "LibCircusCS.h"
#include "visualization.h"

#define RESULT_FILE_NAME  "visualization_v.1.txt"
#define LOG_FILE_NAME     "job.log"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int 
	visualizationMain(char* jobRootPath, int coreNum)
{
	char inHeaderFileName[1024], logFileName[1024];
	char axialFileName[1024], coroFileName[1024], sagiFileName[1024];
	char buffer[1024];

	sprintf(inHeaderFileName, "%s\\0.mhd", jobRootPath);
	sprintf(logFileName,      "%s\\%s",    jobRootPath, LOG_FILE_NAME);

	//------------------------------------------------------------------------------------------------------------------
	// Load volume data from metaheader file 
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFileName, "Load volume data from metaheader file");

	int                voxelUnit;
	CircusCS_INTSIZE3D matrixSize;
	CircusCS_SIZE3D    voxelSize;

	short* volume = (short*)CircusCS_LoadVolumeDataFromMetaHeader(inHeaderFileName,
		&voxelUnit,
		&matrixSize,
		&voxelSize);

	if(volume == NULL)
	{
		sprintf(buffer, "Failed to load volume data (%s)", inHeaderFileName);
		CircusCS_AppendLogFile(logFileName, buffer);
		return -1;
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Create MIP image
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFileName, "Create MIP images");

	// Create MIP image
	short* axialMip = CircusCS_CreateIntensityProjection<short>(volume,
		&matrixSize,
		MAX_INTENSITY,
		AXIAL_SECTION);
	short* coroMip  = CircusCS_CreateIntensityProjection<short>(volume,
		&matrixSize,
		MAX_INTENSITY,
		CORONAL_SECTION);
	short* sagiMip  = CircusCS_CreateIntensityProjection<short>(volume,
		&matrixSize,
		MAX_INTENSITY,
		SAGITTAL_SECTION);

	// Set window level and window width (convert to Uint8) 
	int length = matrixSize.width * matrixSize.height;
	unsigned char* axialImg = CircusCS_SetWindowAndConvertToUint8Image<short>(axialMip, length, 0, 0);

	length = matrixSize.width * matrixSize.depth;
	unsigned char* coroImg = CircusCS_SetWindowAndConvertToUint8Image<short>(coroMip, length, 0, 0);

	length = matrixSize.height * matrixSize.depth;
	unsigned char* sagiImg  = CircusCS_SetWindowAndConvertToUint8Image<short>(sagiMip, length, 0, 0);

	free(axialMip);
	free(coroMip);
	free(sagiMip);
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Save as JPEG file
	//------------------------------------------------------------------------------------------------------------------
	sprintf(axialFileName, "%s\\MIP_axial.jpg",    jobRootPath);
	sprintf(coroFileName,  "%s\\MIP_coronal.jpg",  jobRootPath);
	sprintf(sagiFileName,  "%s\\MIP_sagittal.jpg", jobRootPath);

	CircusCS_SaveImageAsJpeg(axialFileName,
		axialImg,
		matrixSize.width,
		matrixSize.height);
	CircusCS_SaveImageAsJpeg(coroFileName,
		coroImg,
		matrixSize.width,
		matrixSize.depth);
	CircusCS_SaveImageAsJpeg(sagiFileName,
		sagiImg,
		matrixSize.height,
		matrixSize.depth);

	free(axialImg);
	free(coroImg);
	free(sagiImg);
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Save result file to register database
	//------------------------------------------------------------------------------------------------------------------
	sprintf(buffer, "Save result file to register database (%s)", RESULT_FILE_NAME);
	CircusCS_AppendLogFile(logFileName, buffer);
	{
		char fileName[1024];
		sprintf(fileName, "%s\\%s", jobRootPath, RESULT_FILE_NAME);

		FILE* fp = fopen(fileName, "w");
		fprintf(fp, "1\n");

		fclose(fp);
	}
	//------------------------------------------------------------------------------------------------------------------

	CircusCS_AppendLogFile(logFileName, "Finished");

	free(volume);

	return 0;
}


