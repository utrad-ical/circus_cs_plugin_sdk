#pragma warning(disable:4996)

#include <stdio.h>

#include "VOL.h"
#include "LibCircusCS.h"

#include "visualization.h"

#define LOG_FNAME      "job.log"
#define RESULTS_FNAME  "visualization_v.1.txt"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int 
visualizationMain(char* jobRootPath, int coreNum)
{
	char inVolumeFname[1024], inDumpFname[1024], logFname[1024];
	char axialFname[1024], coroFname[1024], sagiFname[1024];

	sprintf(inVolumeFname,  "%s\\0.vol", jobRootPath);
	sprintf(inDumpFname,    "%s\\0.txt", jobRootPath);
	sprintf(logFname,       "%s\\%s",    jobRootPath, LOG_FNAME);
 
	// Load volume data
	VOL_RAWVOLUMEDATA* volume = CircusCS_LoadVolumeOneFile(inVolumeFname);
	if(volume == NULL)
	{
		CircusCS_AppendLogFile(logFname, "Fail to load volume data: 0.vol");
		return -1;
	}

	
	// Rescale half size
	CircusCS_AppendLogFile(logFname, "Rescale half size");
	VOL_INTSIZE3D dstSize;
	dstSize.width  = volume->matrixSize->width  / 2;
	dstSize.height = volume->matrixSize->height / 2;
	dstSize.depth  = volume->matrixSize->depth  / 2;
	VOL_ScaleRawVolumeData(volume, &dstSize, VOL_SCALING_METHOD_LINEAR);
	
	
	CircusCS_AppendLogFile(logFname, "Create MIP images");

	// Create MIP image
	VOL_RAWIMAGEDATA* axialImg = CircusCS_CreateIntensityProjection(volume, 0, MAX_INTENSITY, AXIAL_SECTION);
	VOL_RAWIMAGEDATA* coroImg  = CircusCS_CreateIntensityProjection(volume, 0, MAX_INTENSITY, CORONAL_SECTION);
	VOL_RAWIMAGEDATA* sagiImg  = CircusCS_CreateIntensityProjection(volume, 0, MAX_INTENSITY, SAGITTAL_SECTION);

	// Set window level and window width (convert to UINT8) 
	CircusCS_SetWindowAndConvertToUint8Image(axialImg, 0, 0, 0);
	CircusCS_SetWindowAndConvertToUint8Image(coroImg,  0, 0, 0);
	CircusCS_SetWindowAndConvertToUint8Image(sagiImg,  0, 0, 0);

	// Set file name
	sprintf(axialFname, "%s\\MIP_axial.jpg",    jobRootPath);
	sprintf(coroFname,  "%s\\MIP_coronal.jpg",  jobRootPath);
	sprintf(sagiFname,  "%s\\MIP_sagittal.jpg", jobRootPath);

	// Save as JPEG file
	CircusCS_SaveJPEG(axialImg, 0, axialFname);
	CircusCS_SaveJPEG(coroImg,  0, coroFname);
	CircusCS_SaveJPEG(sagiImg,  0, sagiFname);

	VOL_DeleteRawImageData(axialImg);
	VOL_DeleteRawImageData(coroImg);
	VOL_DeleteRawImageData(sagiImg);

	VOL_DeleteRawVolumeData(volume);


	// Save result file to register database
	char fileName[1024];
	sprintf(fileName, "%s\\%s", jobRootPath, RESULTS_FNAME);
	
	FILE* fp = fopen(fileName, "w");
	fprintf(fp, "1\n");
	fprintf(fp, "1\n");

	fclose(fp);


	CircusCS_AppendLogFile(logFname, "Finished");

	return 0;
}


