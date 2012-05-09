#pragma warning(disable:4996)

#include <stdio.h>

#include "LibCircusCS.h"

#include "measurement.h"
#include "export.h"

#define THRESHOLD	100

#define RESULTS_FNAME     "measurement_v.1.txt"
#define LOG_FNAME		  "job.log"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int
measurmentMain(char* jobRootPath, int coreNum)
{
	char inVolumeFname[1024], inDumpFname[1024], logFname[1024];
	char resFname[1024];
	char buffer[1024];

	sprintf(inVolumeFname, "%s\\0.raw", jobRootPath);
	sprintf(inDumpFname,   "%s\\0.txt", jobRootPath);
	sprintf(resFname,      "%s\\%s",    jobRootPath, RESULTS_FNAME);	
	sprintf(logFname,      "%s\\%s",    jobRootPath, LOG_FNAME);

	//------------------------------------------------------------------------------------------------------------------
	// Get basic DICOM tag value
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Load DICOM dump data");

	CircusCS_BASICDCMTAGVALUES* dcmTagData = CircusCS_NewBasicDcmTagValues(inDumpFname);
	if(dcmTagData == NULL)
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

	int length = dcmTagData->matrixSize->width * dcmTagData->matrixSize->height * dcmTagData->matrixSize->depth;
	short* volume = CircusCS_LoadRawVolumeFileAsSint16(inVolumeFname, length);

	if(volume == NULL)
	{
		sprintf(buffer, "Fail to load volume data: %s", inVolumeFname);
		CircusCS_AppendLogFile(logFname, buffer);
		CircusCS_DeleteBasicDcmTagValues(dcmTagData);
		return -1;
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Initialize result volume (RGBA color)
	//------------------------------------------------------------------------------------------------------------------
	unsigned char* resultVolume = (unsigned char*)calloc(length*3, sizeof(unsigned char));

	if(resultVolume == NULL)
	{
		sprintf(buffer, "Fail to allocate result volume");
		CircusCS_AppendLogFile(logFname, buffer);
		free(volume);
		return -1;
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Measurement main
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Measurement main");

	double min  = 30000.0;
	double max  = -30000.0;
	double mean = 0.0;

	short threshold = 100;

	for(int k=0; k<dcmTagData->matrixSize->depth;  k++)
	for(int j=0; j<dcmTagData->matrixSize->height; j++)
	for(int i=0; i<dcmTagData->matrixSize->width;  i++)
	{
		int pos = k * dcmTagData->matrixSize->height * dcmTagData->matrixSize->width
			    + j * dcmTagData->matrixSize->width + i;

		if(volume[pos] < min) min  = volume[pos];
		if(volume[pos] > max) max  = volume[pos];
		mean += volume[pos];

		resultVolume[pos * 3] = resultVolume[pos * 3 + 1] = resultVolume[pos * 3 + 2] = 0;

		if(volume[pos] >= THRESHOLD)
		{
			resultVolume[pos * 3] = resultVolume[pos * 3 + 1] = 255;
		}
	}
	mean /= length;
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Save measurement results (measurement_v.1.txt)
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Save measurement results (measurement_v.1.txt)");
	{
		FILE* fp = fopen(resFname, "w");
		if(!fp)  return -1;

		fprintf(fp, "%d, %.2f, %.2f, %.2f\n", 1, min, max, mean);

		fclose(fp);
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Export image files from volume data (axial section) 
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Export image files from volume data (axial section)");

	exportImageFilesFromVolumeData(jobRootPath, volume, resultVolume, dcmTagData->matrixSize);

	free(volume);
	free(resultVolume);
	//------------------------------------------------------------------------------------------------------------------

	CircusCS_AppendLogFile(logFname, "Finished");

	return 0;
}


