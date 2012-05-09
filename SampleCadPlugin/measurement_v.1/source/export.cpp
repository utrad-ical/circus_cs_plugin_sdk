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
							   short* orgVolume,
							   unsigned char* resultVolume,
							   CircusCS_INTSIZE3D* matrixSize)
{

	//------------------------------------------------------------------------------------------------------------------
	//  Export image files (as PNG file)
	//------------------------------------------------------------------------------------------------------------------
	char orgFname[1024], resFname[1024];
	int  length = matrixSize->width * matrixSize->height;

	for(int k=0; k<matrixSize->depth; k++)
	{
		short*  orgImg = CircusCS_ExtractSingleSliceFromVolumeDataAsSint16(orgVolume, matrixSize, k, AXIAL_SECTION);

		// Set window level and window width (original data) 
		unsigned char* orgImgUint8 = CircusCS_SetWindowAndConvertToUint8ImageFromSint16(orgImg,
																						length,
																						WINDOW_LEVEL,
																						WINDOW_WIDTH);
		// Export original image
		sprintf(orgFname, "%s\\org%04d.png", jobRootPath, k+1);
		CircusCS_SaveImageAsPng(orgFname, orgImgUint8, matrixSize->width, matrixSize->height);

		free(orgImg);
		free(orgImgUint8);
		

		unsigned char* resultImg = (unsigned char*)calloc(length*3, sizeof(unsigned char));
			
		for(int j=0; j<matrixSize->height; j++)
		for(int i=0; i<matrixSize->width;  i++)
		{
			int pos2D = j*matrixSize->width + i;
			int pos3D = k*matrixSize->height*matrixSize->width + j*matrixSize->width + i;

			resultImg[pos2D * 3]     = resultVolume[pos3D * 3]; 
			resultImg[pos2D * 3 + 1] = resultVolume[pos3D * 3 + 1]; 
			resultImg[pos2D * 3 + 2] = resultVolume[pos3D * 3 + 2]; 
		}

		// Export result image
		sprintf(resFname, "%s\\result%04d.png", jobRootPath, k+1);
		CircusCS_SaveImageAsPng(resFname, resultImg, matrixSize->width, matrixSize->height, RGB_COLOR);

		free(resultImg);

	} // end for
	//------------------------------------------------------------------------------------------------------------------

	return 0;
}