#pragma warning(disable:4996)

#include <stdio.h>
#include <vector>
#include <string>

#include "LibCircusCS.h"
#include "detector.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RESULT_WINDOW_LEVEL 0
#define RESULT_WINDOW_WIDTH 0

#define ATTRIBUTES_FNAME  "executed_plugin_attributes.txt"
#define RESULTS_FNAME     "detector_v.1.txt"
#define LOG_FNAME		  "job.log"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int
detectorMain(char* jobRootPath, int coreNum)
{
	char inVolumeFname[1024], inDumpFname[1024], logFname[1024], outFname[1024];
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
	// Lesion detection (dummy)
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Lesion detection (dummy)");

	int candidateNum = 5;
	int featureNum = 5;

	std::vector< std::vector<float> > data(candidateNum, std::vector<float>(featureNum));

	float confidence = 0.99f;
	float tmpPos     = -4.0f;

	for(int i=0; i<candidateNum; i++)
	{
		data[i][0] = (float)basicTagValues->matrixSize->width/2.0f + tmpPos;		// location_x	
		data[i][1] = data[i][0] + 5.0f;												// location_y
		data[i][2] = (float)(basicTagValues->matrixSize->depth * (i + 2))/10.0f;	// location_z
		data[i][3] = (float)(i * 10);												// volume [voxels]
		data[i][4] = confidence;													// confidence
	
		confidence *= 0.9f;
		tmpPos *= -2.0f;
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Save attiributes (executed_plugin_attributes.txt)
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Save attiributes (executed_plugin_attributes.txt)");
	{
		sprintf(outFname, "%s\\%s", jobRootPath, ATTRIBUTES_FNAME);
		FILE* fp = fopen(outFname, "w");

		if(fp == NULL)
		{
			sprintf(buffer, "Fail to write attiributes: %s", outFname);
			CircusCS_AppendLogFile(logFname, buffer);
			CircusCS_DeleteBasicDcmTagValues(basicTagValues);
			free(volume);
			return -1;
		}

		fprintf(fp, "voxel_size_x, %f\n", basicTagValues->voxelSize_mm->width);
		fprintf(fp, "voxel_size_y, %f\n", basicTagValues->voxelSize_mm->height);
		fprintf(fp, "voxel_size_z, %f\n", basicTagValues->voxelSize_mm->depth);

		fprintf(fp, "crop_org_x,  %d\n", 0);
		fprintf(fp, "crop_org_y,  %d\n", 0);
		fprintf(fp, "crop_org_z,  %d\n", 0);
		fprintf(fp, "crop_width,  %d\n", basicTagValues->matrixSize->width);
		fprintf(fp, "crop_height, %d\n", basicTagValues->matrixSize->height);
		fprintf(fp, "crop_depth,  %d\n", basicTagValues->matrixSize->depth);
	
		fprintf(fp, "window_level, %d\n", RESULT_WINDOW_LEVEL);
		fprintf(fp, "window_width, %d\n", RESULT_WINDOW_WIDTH);

		fclose(fp);
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Save detector results (detector_v.1.txt)
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Save detector results (detector_v.1.txt)");
	{
		sprintf(outFname, "%s\\%s", jobRootPath, RESULTS_FNAME);
		FILE* fp = fopen(outFname, "w");
	
		if(fp == NULL)
		{
			sprintf(buffer, "Fail to write detector results: %s", outFname);
			CircusCS_AppendLogFile(logFname, buffer);
			CircusCS_DeleteBasicDcmTagValues(basicTagValues);
			free(volume);
			return -1;
		}

		double voxelVolume = basicTagValues->matrixSize->width
							* basicTagValues->matrixSize->height
							* basicTagValues->matrixSize->depth;

		for(int n=0; n<candidateNum; n++)
		{
			// Get slice location of lesion candidate
			float sliceLocation = basicTagValues->sliceLocation_mm[(int)data[n][2]];

			fprintf(fp, "%d, %d, %d, %d, %.2f, %.2f, %f\n", n+1,
								                            (int)data[n][0],
						 									(int)data[n][1],
															(int)data[n][2],
															sliceLocation,
															(double)data[n][3] * voxelVolume,
															data[n][4]);

		} // end for	

		fclose(fp);
	}
	//------------------------------------------------------------------------------------------------------------------

	CircusCS_AppendLogFile(logFname, "Finished");

	free(volume);
	CircusCS_DeleteBasicDcmTagValues(basicTagValues);

	return candidateNum;
}


