#pragma warning(disable:4996)

#include <stdio.h>

#include "LibCircusCS.h"

#include "measurement.h"
#include "export.h"

#define THRESHOLD	100

#define RESULT_FILE_NAME "measurement_v.1.txt"
#define LOG_FILE_NAME	 "job.log"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int
	measurmentMain(char* jobRootPath, int coreNum)
{
	char inVolumeFileName[1024], inDumpFileName[1024], logFileName[1024];
	char resFileName[1024];
	char buffer[1024];

	sprintf(inVolumeFileName, "%s\\0.raw", jobRootPath);
	sprintf(inDumpFileName,   "%s\\0.txt", jobRootPath);
	sprintf(resFileName,      "%s\\%s",    jobRootPath, RESULT_FILE_NAME);	
	sprintf(logFileName,      "%s\\%s",    jobRootPath, LOG_FILE_NAME);

	//------------------------------------------------------------------------------------------------------------------
	// Get basic DICOM tag value
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFileName, "Load DICOM dump data");

	CircusCS_BASICDCMTAGVALUES* basicTagValues = CircusCS_NewBasicDcmTagValues(inDumpFileName);
	if(basicTagValues == NULL)
	{
		sprintf(buffer, "Failed to load DICOM dump data: %s", inDumpFileName);
		CircusCS_AppendLogFile(logFileName, buffer);
		return -1;
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Load volume data
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFileName, "Load volume data");

	int length = basicTagValues->matrixSize->width
		* basicTagValues->matrixSize->height
		* basicTagValues->matrixSize->depth;
	short* volume = CircusCS_LoadRawVolumeFile<short>(inVolumeFileName, length);

	if(volume == NULL)
	{
		sprintf(buffer, "Failed to load volume data: %s", inVolumeFileName);
		CircusCS_AppendLogFile(logFileName, buffer);
		CircusCS_DeleteBasicDcmTagValues(basicTagValues);
		return -1;
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Initialize result volume (RGBA color)
	//------------------------------------------------------------------------------------------------------------------
	unsigned char* resultVolume = (unsigned char*)calloc(length*3, sizeof(unsigned char));

	if(resultVolume == NULL)
	{
		sprintf(buffer, "Failed to allocate result volume");
		CircusCS_AppendLogFile(logFileName, buffer);
		free(volume);
		return -1;
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Measurement main
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFileName, "Measurement main");

	double min  = 30000.0;
	double max  = -30000.0;
	double mean = 0.0;

	short threshold = 100;

	for(int k=0; k<basicTagValues->matrixSize->depth;  k++)
	{
		for(int j=0; j<basicTagValues->matrixSize->height; j++)
		{
			for(int i=0; i<basicTagValues->matrixSize->width;  i++)
			{
				int pos = k * basicTagValues->matrixSize->height * basicTagValues->matrixSize->width
					+ j * basicTagValues->matrixSize->width + i;

				if(volume[pos] < min) min  = volume[pos];
				if(volume[pos] > max) max  = volume[pos];
				mean += volume[pos];

				resultVolume[pos * 3] = resultVolume[pos * 3 + 1] = resultVolume[pos * 3 + 2] = 0;

				if(volume[pos] >= THRESHOLD)
				{
					resultVolume[pos * 3] = resultVolume[pos * 3 + 1] = 255;
				}
			}
		}
	}
	mean /= length;
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Save measurement results
	//------------------------------------------------------------------------------------------------------------------
	sprintf(buffer, "Save measurement results (%s)", RESULT_FILE_NAME);

	CircusCS_AppendLogFile(logFileName, buffer);
	{
		FILE* fp = fopen(resFileName, "w");
		if(!fp)  return -1;

		fprintf(fp, "%d, %.2f, %.2f, %.2f\n", 1, min, max, mean);
		fclose(fp);
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Export image files from volume data (axial section) 
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFileName, "Export image files from volume data (axial section)");

	exportImageFilesFromVolumeData(jobRootPath, volume, resultVolume, basicTagValues->matrixSize);

	free(volume);
	free(resultVolume);
	CircusCS_DeleteBasicDcmTagValues(basicTagValues);
	//------------------------------------------------------------------------------------------------------------------

	CircusCS_AppendLogFile(logFileName, "Finished");

	return 0;
}


