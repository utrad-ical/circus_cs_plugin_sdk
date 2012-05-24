#pragma warning(disable:4996)

#include <stdio.h>

#include "LibCircusCS.h"
#include "visualization.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOG_FNAME      "job.log"
#define RESULTS_FNAME  "visualization_v.1.txt"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int 
visualizationMain(char* jobRootPath, int coreNum)
{
	char inVolumeFname[1024], inDumpFname[1024], logFname[1024];
	char axialFname[1024], coroFname[1024], sagiFname[1024];
	char buffer[1024];

	sprintf(inVolumeFname,  "%s\\0.raw", jobRootPath);
	sprintf(inDumpFname,    "%s\\0.txt", jobRootPath);
	sprintf(logFname,       "%s\\%s",    jobRootPath, LOG_FNAME);

	//------------------------------------------------------------------------------------------------------------------
	// Get basic DICOM tag value
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Load DICOM dump data");

	CircusCS_BASICDCMTAGVALUES* basicTagValues = CircusCS_NewBasicDcmTagValues(inDumpFname);
	if(basicTagValues == NULL)
	{
		sprintf(buffer, "Fail to load DICOM dump data: %s", inDumpFname);
		CircusCS_AppendLogFile(logFname, buffer);
		return -1;
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Load volume data
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Load volume data");

	int length = basicTagValues->matrixSize->width
			   * basicTagValues->matrixSize->height
			   * basicTagValues->matrixSize->depth;
	short* volume = CircusCS_LoadRawVolumeFile<short>(inVolumeFname, length);

	if(volume == NULL)
	{
		sprintf(buffer, "Fail to load volume data: %s", inVolumeFname);
		CircusCS_AppendLogFile(logFname, buffer);
		CircusCS_DeleteBasicDcmTagValues(basicTagValues);
		return -1;
	}
	//------------------------------------------------------------------------------------------------------------------
	
	//------------------------------------------------------------------------------------------------------------------
	// Create MIP image
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Create MIP images");

	// Create MIP image
	short* axialMip = CircusCS_CreateIntensityProjection<short>(volume,
																basicTagValues->matrixSize,
																MAX_INTENSITY,
																AXIAL_SECTION);
	short* coroMip  = CircusCS_CreateIntensityProjection<short>(volume,
		                                                        basicTagValues->matrixSize,
																MAX_INTENSITY,
																CORONAL_SECTION);
	short* sagiMip  = CircusCS_CreateIntensityProjection<short>(volume,
		                                                        basicTagValues->matrixSize,
																MAX_INTENSITY,
																SAGITTAL_SECTION);

	// Set window level and window width (convert to Uint8) 
	length = basicTagValues->matrixSize->width
		   * basicTagValues->matrixSize->height;
	unsigned char* axialImg = CircusCS_SetWindowAndConvertToUint8Image<short>(axialMip, length, 0, 0);

	length = basicTagValues->matrixSize->width
		   * basicTagValues->matrixSize->depth;
	unsigned char* coroImg = CircusCS_SetWindowAndConvertToUint8Image<short>(coroMip, length, 0, 0);

	length = basicTagValues->matrixSize->height
		   * basicTagValues->matrixSize->depth;
	unsigned char* sagiImg  = CircusCS_SetWindowAndConvertToUint8Image<short>(sagiMip, length, 0, 0);

	free(axialMip);
	free(coroMip);
	free(sagiMip);
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Save as JPEG file
	//------------------------------------------------------------------------------------------------------------------
	sprintf(axialFname, "%s\\MIP_axial.jpg",    jobRootPath);
	sprintf(coroFname,  "%s\\MIP_coronal.jpg",  jobRootPath);
	sprintf(sagiFname,  "%s\\MIP_sagittal.jpg", jobRootPath);

	CircusCS_SaveImageAsJpeg(axialFname,
							 axialImg,
							 basicTagValues->matrixSize->width,
							 basicTagValues->matrixSize->height);
	CircusCS_SaveImageAsJpeg(coroFname,
							 coroImg,
							 basicTagValues->matrixSize->width,
							 basicTagValues->matrixSize->depth);
	CircusCS_SaveImageAsJpeg(sagiFname,
							 sagiImg,
							 basicTagValues->matrixSize->height,
							 basicTagValues->matrixSize->depth);

	free(axialImg);
	free(coroImg);
	free(sagiImg);
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Save result file to register database
	//------------------------------------------------------------------------------------------------------------------
	{
		char fileName[1024];
		sprintf(fileName, "%s\\%s", jobRootPath, RESULTS_FNAME);
	
		FILE* fp = fopen(fileName, "w");
		fprintf(fp, "1\n");

		fclose(fp);
	}
	//------------------------------------------------------------------------------------------------------------------

	CircusCS_AppendLogFile(logFname, "Finished");

	free(volume);
	CircusCS_DeleteBasicDcmTagValues(basicTagValues);

	return 0;
}


