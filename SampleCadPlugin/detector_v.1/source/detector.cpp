#pragma warning(disable:4996)

#include <string>
#include <stdio.h>

#include "VOL.h"
#include "LibCircusCS.h"

#include "detector.h"

#define  LOG_FNAME		"job.log"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOL_MATRIX*
detectorMain(char* jobRootPath, VOL_INTBOX3D* cropBox, int coreNum)
{
	char inVolumeFname[1024], inDumpFname[1024], logFname[1024];

	sprintf(inVolumeFname,  "%s\\0.vol", jobRootPath);
	sprintf(inDumpFname,    "%s\\0.txt", jobRootPath);
	sprintf(logFname,       "%s\\%s",    jobRootPath, LOG_FNAME);

	CircusCS_AppendLogFile(logFname, "Load volume data");

	// Load volume data
	VOL_RAWVOLUMEDATA* volume = CircusCS_LoadVolumeOneFile(inVolumeFname);
	if(volume == NULL)
	{
		CircusCS_AppendLogFile(logFname, "Fail to load volume data: 0.vol");
		return NULL;
	}

	// Get basic DICOM tag value
	CircusCS_BASICDCMTAGVALUES* dcmTagData = CircusCS_NewBasicDcmTagValues(inDumpFname);
	if(dcmTagData == NULL)
	{
		CircusCS_AppendLogFile(logFname, "Fail to load DICOM dump data: 0.txt");
		VOL_DeleteRawVolumeData(volume);
		return NULL;
	}

	//-------------------------------------------------------------------------------------------------------
	// Cropping (dummy)
	//-------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Cropping (dummy)");

	// set cropBox for cropping
	int cropMargin = 10;
	cropBox->origin->x = cropMargin;
	cropBox->origin->y = cropMargin;
	cropBox->origin->z = cropMargin;
	cropBox->size->width  = volume->matrixSize->width - cropMargin*2;
	cropBox->size->height = volume->matrixSize->height - cropMargin*2;
	cropBox->size->depth  = volume->matrixSize->depth - cropMargin*2;

	VOL_ResizeRawVolumeData(volume, cropBox, VOL_RESIZE_BACKGROUNDTYPE_ZERO);
	//-------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------
	// Lesion detection (dummy)
	//-------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Lesion detection (dummy)");

	int candNum = 5;
	int featureNum = 5;
	VOL_MATRIX* ret = VOL_NewMatrix(candNum, featureNum);

	float confidence = 0.99f;
	float tmpPos = -4.0f;

	for(int i=0; i<candNum; i++)
	{
		ret->data[i][0] = (float)volume->matrixSize->width/2.0f + tmpPos
						+ (float)cropBox->origin->x; 								// location_x
		ret->data[i][1] = (ret->data[i][0] + 5.0f) + (float)cropBox->origin->y;		// location_y
		ret->data[i][2] = (float)(volume->matrixSize->depth * (i + 2))/10.0f
						+ (float)cropBox->origin->z;			                    // location_z
		ret->data[i][3] = (float)(i * 10);											// volume [voxels]
		ret->data[i][4] = confidence;												// confidence
	
		confidence *= 0.9f;
		tmpPos *= -2.0f;
	}
	//-------------------------------------------------------------------------------------------------------

	CircusCS_DeleteBasicDcmTagValues(dcmTagData);
	VOL_DeleteRawVolumeData(volume);

	return ret;
}


