#pragma warning(disable:4996)

#include <stdio.h>
#include <math.h>

#include <string>
#include <vector>

#include "VOL.h"
#include "LibCircusCS.h"

#include "export.h"

#define WINDOW_LEVEL	0
#define WINDOW_WIDTH	0


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int
exportImageFilesFromVolumeData(char* jobRootPath,
							   VOL_RAWVOLUMEDATA* orgVolume,
							   VOL_RAWVOLUMEDATA* resultVolume)
{

	if(orgVolume->matrixSize->depth != resultVolume->matrixSize->depth)
	{
		return -1;
	}

	//------------------------------------------------------------------------------------------------------------------
	//  Export image files
	//------------------------------------------------------------------------------------------------------------------
	char orgFname[1024], resFname[1024];

	VOL_INTSIZE3D* orgMatrix = VOL_GetIntSize3DFromIntSize4D(orgVolume->matrixSize);

	for(int k=0; k<orgMatrix->depth; k++)
	{
		VOL_RAWIMAGEDATA* orgImg = CircusCS_ExtractSingleSliceFromRawVolumeData(orgVolume,    0, k, AXIAL_SECTION);
		VOL_RAWIMAGEDATA* resImg = CircusCS_ExtractSingleSliceFromRawVolumeData(resultVolume, 0, k, AXIAL_SECTION);

		// Set window level and window width (original data) 
		CircusCS_SetWindowAndConvertToUint8Image(orgImg, 0, WINDOW_LEVEL, WINDOW_WIDTH);

		// Set file name
		sprintf(orgFname, "%s\\org%04d.png",    jobRootPath, k+1);
		sprintf(resFname, "%s\\result%04d.png", jobRootPath, k+1);
		
		// Export as PNG file
		CircusCS_SavePNG(orgImg, 0, orgFname);
		CircusCS_SavePNG(resImg, 0, resFname);

		VOL_DeleteRawImageData(orgImg);
		VOL_DeleteRawImageData(resImg);

	} // end for
	//------------------------------------------------------------------------------------------------------------------

	return 0;
}