#pragma warning(disable:4996)

#include <stdio.h>

#include "VOL.h"
#include "LibCircusCS.h"

#include "measurement.h"
#include "export.h"

#define THRESHOLD	100

#define RESULTS_FNAME     "measurement_v.1.txt"
#define LOG_FNAME		  "job.log"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long get_RGBA_bytes(float r,float g,float b,float a)
{
	unsigned long	R, G, B, A;

	R = static_cast<unsigned long>(r*255.0f);
	G = static_cast<unsigned long>(g*255.0f);
	B = static_cast<unsigned long>(b*255.0f);
	A = static_cast<unsigned long>(a*255.0f);

	return ( (R<<24) + (G<<16) + (B<<8) + A );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int
measurmentMain(char* jobRootPath, int coreNum)
{
	char inVolumeFname[1024], resFname[1024], logFname[1024];

	sprintf(inVolumeFname, "%s\\0.vol", jobRootPath);
	sprintf(resFname,      "%s\\%s",    jobRootPath, RESULTS_FNAME);	
	sprintf(logFname,      "%s\\%s",    jobRootPath, LOG_FNAME);
 
	// Load volume data
	VOL_RAWVOLUMEDATA* volume = CircusCS_LoadVolumeOneFile(inVolumeFname);
	if(volume == NULL)
	{
		CircusCS_AppendLogFile(logFname, "Fail to load volume data: 0.vol");
		return -1;
	}

	// Initialize result volume (RGBA color)
	VOL_INTSIZE3D* matrixSize = VOL_GetIntSize3DFromIntSize4D(volume->matrixSize);

	VOL_RAWVOLUMEDATA* resultVolume = VOL_NewSingleChannelRawVolumeData(matrixSize, VOL_VALUEUNIT_UINT32,VOL_VALUETYPE_RGBA);
	if(resultVolume == NULL)
	{
		VOL_DeleteRawVolumeData(volume);
		return -1;
	}

	//------------------------------------------------------------------------------------------------------------------
	// Measurement main
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Measurement main");

	double min  = 30000.0;
	double max  = -30000.0;
	double mean = 0.0;

	short threshold = 100;

	short***         srcData = (short***)volume->array4D[0];
	unsigned long*** dstData = (unsigned long***)resultVolume->array4D[0];

	for(int k=0; k<matrixSize->depth;  k++)
	for(int j=0; j<matrixSize->height; j++)
	for(int i=0; i<matrixSize->width;  i++)
	{
		if(srcData[k][j][i] < min) min  = srcData[k][j][i];
		if(srcData[k][j][i] > max) max  = srcData[k][j][i];
		mean += srcData[k][j][i];

		if(srcData[k][j][i] >= THRESHOLD)
		{
			dstData[k][j][i] = get_RGBA_bytes(1.0f, 1.0f, 0.0f, 0.0f);
		}
	}
	mean /= (matrixSize->width * matrixSize->height * matrixSize->depth);
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Save measurement results (measurement_v.1.txt)
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Save measurement results (measurement_v.1.txt)");

	FILE* fp = fopen(resFname, "w");
	if(!fp)  return -1;

	// 1st line
	fprintf(fp, "%d\n", 1);

	// 2nd line 
	fprintf(fp, "%d, %.2f, %.2f, %.2f\n", 1, min, max, mean);

	fclose(fp);
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Export image files from volume data (axial section) 
	//------------------------------------------------------------------------------------------------------------------
	CircusCS_AppendLogFile(logFname, "Export image files from volume data (axial section)");

	exportImageFilesFromVolumeData(jobRootPath, volume, resultVolume);

	VOL_DeleteRawVolumeData(volume);
	VOL_DeleteRawVolumeData(resultVolume);
	//------------------------------------------------------------------------------------------------------------------

	CircusCS_AppendLogFile(logFname, "Finished");

	return 0;
}


