#pragma warning(disable:4996)

#include <stdio.h>
#include <math.h>

#include <string>
#include <vector>

#include "VOL.h"
#include "LibCircusCS.h"

#include "results.h"

#define RESULT_WINDOW_LEVEL 0
#define RESULT_WINDOW_WIDTH 0

#define ATTRIBUTES_FNAME  "executed_plugin_attributes.txt"
#define RESULTS_FNAME     "detector_v.1.txt"
#define LOG_FNAME		  "job.log"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int
createResultFiles(char* jobRootPath, VOL_MATRIX* detectResult, VOL_INTBOX3D* cropBox)
{
	char fileName[1024], dumpFname[1024], logFname[1024];

	sprintf(logFname, "%s\\%s", jobRootPath, LOG_FNAME);

	// Load volume data
	sprintf(fileName, "%s\\0.vol", jobRootPath);
	VOL_RAWVOLUMEDATA* volume = CircusCS_LoadVolumeOneFile(fileName);
	int  orgSliceNum = volume->matrixSize->depth;
	
	// Load DICOM dump file
	sprintf(fileName, "%s\\0.txt", jobRootPath);
	CircusCS_DCMDUMPDATA* dcmDumpData = CircusCS_LoadDcmDumpFile(fileName);

	//------------------------------------------------------------------------------------------------------------------
	// Get attiributes from DICOM dump
	//------------------------------------------------------------------------------------------------------------------
	int startSliceNum = 0;
	int endSliceNum = 0;
	float sliceLocationOrigin_mm;
	float sliceLocationPitch_mm;
	VOL_SIZE3D voxelSize_mm;

	// Get voxel size
	CircusCS_GetVoxelSizeOfDumpData(dcmDumpData, &voxelSize_mm);

	// Get origin and pitch of slice location
	{
		float sliceLocation0, sliceLocation1;	

		CircusCS_GetSliceLocationOfDumpData(dcmDumpData, &sliceLocation0, 0);
		CircusCS_GetSliceLocationOfDumpData(dcmDumpData, &sliceLocation1, 1);

		sliceLocationOrigin_mm = sliceLocation0;
		sliceLocationPitch_mm = sliceLocation1 - sliceLocation0;
	}

	// Get start / end slice number
	CircusCS_GetImageNumberOfDumpData(dcmDumpData, &startSliceNum, 0);
	CircusCS_GetImageNumberOfDumpData(dcmDumpData, &endSliceNum, orgSliceNum-1);	
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Save attiributes (executed_plugin_attributes.txt)
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Save attiributes (executed_plugin_attributes.txt)");
	sprintf(fileName, "%s\\%s", jobRootPath, ATTRIBUTES_FNAME);

	FILE* fp = fopen(fileName, "w");
	if(!fp)  return -1;

	if(startSliceNum > 1)
	{
		fprintf(fp, "13\n");
		fprintf(fp, "'start_img_num', %d\n", startSliceNum);
		fprintf(fp, "'end_img_num', %d\n",   endSliceNum);
	}
	else
	{
		fprintf(fp, "11\n");
	}

	fprintf(fp, "'voxel_size_x', %f\n", voxelSize_mm.width);
	fprintf(fp, "'voxel_size_y', %f\n", voxelSize_mm.height);
	fprintf(fp, "'voxel_size_z', %f\n", voxelSize_mm.depth);

	fprintf(fp, "'crop_org_x',  %d\n", cropBox->origin->x);
	fprintf(fp, "'crop_org_y',  %d\n", cropBox->origin->y);
	fprintf(fp, "'crop_org_z',  %d\n", cropBox->origin->z+1);
	fprintf(fp, "'crop_width',  %d\n", cropBox->size->width);
	fprintf(fp, "'crop_height', %d\n", cropBox->size->height);
	fprintf(fp, "'crop_depth',  %d\n", cropBox->size->depth);

	fprintf(fp, "'window_level', %d\n", RESULT_WINDOW_LEVEL);
	fprintf(fp, "'window_width', %d\n", RESULT_WINDOW_WIDTH);

	fclose(fp);
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Save detector results (detector_v.1.txt)
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Save detector results (detector_v.1.txt)");
	
	sprintf(dumpFname, "%s\\0.txt", jobRootPath);
	sprintf(fileName, "%s\\%s", jobRootPath, RESULTS_FNAME);

	fp = fopen(fileName, "w");
	if(!fp)  return -1;

	// 1st line
	int candNum = detectResult->m;
	fprintf(fp, "%d\n", candNum);

	double voxelVolume = voxelSize_mm.width * voxelSize_mm.height * voxelSize_mm.depth;

	for(int n=0; n<candNum; n++)
	{
		// Get image number / slice location of lesion candidate from DICOM dump
		int imgNum = 0;
		float sliceLocation = 0.0f;
			
		CircusCS_GetImageNumberOfDumpData(dcmDumpData, &imgNum, (int)detectResult->data[n][2]);
		CircusCS_GetSliceLocationOfDumpData(dcmDumpData, &sliceLocation, (int)detectResult->data[n][2]);

		fprintf(fp, "%d, %d, %d, %d, %.2f, %.2f, %f\n", n+1,
							                            (int)detectResult->data[n][0],
					 									(int)detectResult->data[n][1],
														imgNum,
														sliceLocation,
														(double)detectResult->data[n][3] * voxelVolume,
														detectResult->data[n][4]);

	} // end for	

	fclose(fp);
	//------------------------------------------------------------------------------------------------------------------

	VOL_DeleteRawVolumeData(volume);
	CircusCS_DeleteDcmDumpData(dcmDumpData);

	CircusCS_AppendLogFile(logFname, "Finished");

	return 0;
}