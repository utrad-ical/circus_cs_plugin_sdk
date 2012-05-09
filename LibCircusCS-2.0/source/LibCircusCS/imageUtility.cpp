////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			imageUtility.cpp : utility for VOL_RAWIMAGEDATA 
//
//			main coder: Y. Nomura
//
//			update history
//
//			2011.03.23	start coding
//			2012.05.08  rewrite without VOL library
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "../LibCircusCS.h"

#include "imageUtility.private.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CircusCS_INTSIZE2D*
CircusCS_GetImageSizeBySection(CircusCS_INTSIZE3D* matrix3D, int section)
{
	CircusCS_INTSIZE2D* ret = CircusCS_NewIntSize2D(0, 0);

	switch(section)
	{
		case AXIAL_SECTION:  // Axial
			ret->width  = matrix3D->width;
			ret->height = matrix3D->height;
			break;

		case CORONAL_SECTION:  // Coronal
			ret->width  = matrix3D->width;
			ret->height = matrix3D->depth;
			break;
			
		case SAGITTAL_SECTION:  // Sagittal
			ret->width  = matrix3D->height;
			ret->height = matrix3D->depth;
			break;
	} // end switch

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename VARTYPE> VARTYPE* 
ExtractSingleSliceFromRawVolumeData(VARTYPE* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section)
{
	VARTYPE*            ret = NULL;
	CircusCS_INTSIZE2D* matrix2D = CircusCS_GetImageSizeBySection(matrix3D, section);

	if((ret = (VARTYPE*)calloc(matrix2D->width * matrix2D->height, sizeof(VARTYPE))) == NULL) return NULL;
	
	int pos2D, pos3D;

	switch(section)
	{
		case AXIAL_SECTION: // Axial
			{
				for(int j=0; j<matrix3D->height; j++)
				for(int i=0; i<matrix3D->width;  i++)
				{
					pos2D = j*matrix2D->width + i;
					pos3D = sliceNum*matrix3D->height*matrix3D->width + j*matrix3D->width + i;

					ret[pos2D] = volume[pos3D]; 
				}
				break;
			}
			
		case CORONAL_SECTION: // Coronal
			{
				for(int k=0; k<matrix3D->depth; k++)
				for(int i=0; i<matrix3D->width; i++)
				{
					pos2D = k*matrix2D->width + i;
					pos3D = k*matrix3D->height*matrix3D->width + sliceNum*matrix3D->width + i;
					ret[pos2D] = volume[pos3D];
				}
				break;
			}

		case SAGITTAL_SECTION: // Sagital
			{
				for(int k=0; k<matrix3D->depth;  k++)
				for(int j=0; j<matrix3D->height; j++)
				{
					pos2D = k*matrix2D->width + j;
					pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width + sliceNum;
					ret[pos2D] = volume[pos3D]; 
				}
				break;
			}
	} // end switch

	return ret;
}

unsigned char*
CircusCS_ExtractSingleSliceFromVolumeDataAsUint8(unsigned char* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section)
{
	return ExtractSingleSliceFromRawVolumeData<unsigned char>(volume, matrix3D, sliceNum, section);
}

char*
CircusCS_ExtractSingleSliceFromVolumeDataAsSint8(char* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section)
{
	return ExtractSingleSliceFromRawVolumeData<char>(volume, matrix3D, sliceNum, section);
}

unsigned short*
CircusCS_ExtractSingleSliceFromVolumeDataAsUint16(unsigned short* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section)
{
	return ExtractSingleSliceFromRawVolumeData<unsigned short>(volume, matrix3D, sliceNum, section);
}

short*
CircusCS_ExtractSingleSliceFromVolumeDataAsSint16(short* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section)
{
	return ExtractSingleSliceFromRawVolumeData<short>(volume, matrix3D, sliceNum, section);
}

unsigned int*
CircusCS_ExtractSingleSliceFromVolumeDataAsUint32(unsigned int* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section)
{
	return ExtractSingleSliceFromRawVolumeData<unsigned int>(volume, matrix3D, sliceNum, section);
}

int*
CircusCS_ExtractSingleSliceFromVolumeDataAsSint32(int* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section)
{
	return ExtractSingleSliceFromRawVolumeData<int>(volume, matrix3D, sliceNum, section);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename VARTYPE> unsigned char* 
SetWindowAndConvertToUint8Image(VARTYPE* img, int length, int windowLevel, int windowWidth)
{
	unsigned char* ret = NULL;
	if((ret = (unsigned char*)calloc(length, sizeof(unsigned char))) == NULL) return NULL;

	double min = (double)windowLevel - (double)windowWidth * 0.5f;
	double max = (double)windowLevel + (double)windowWidth * 0.5f;

	if(windowLevel == 0 && windowWidth == 0)
	{
		min = img[0];
		max = img[0];

		for(int i=1; i<length; i++)
		{
			if(min > img[i])  min = img[i];
			if(max < img[i])  max = img[i];
		}	
	}

	for(int i=1; i<length; i++)
	{
		if(img[i] <= min)      ret[i] = 0;
		else if(img[i] <= min) ret[i] = 255;
		else
		{
			double tmp = 255.0 * (img[i] - min) / (max - min); 
			ret[i] = (unsigned char)(tmp + 0.5);
		}
	}

	return ret;
}

unsigned char*
CircusCS_SetWindowAndConvertToUint8ImageFromUint8(unsigned char* img, int length, int windowLevel, int windowWidth)
{
	return SetWindowAndConvertToUint8Image<unsigned char>(img, length, windowLevel, windowWidth);
}


unsigned char*
CircusCS_SetWindowAndConvertToUint8ImageFromSint8(char* img, int length, int windowLevel, int windowWidth)
{
	return SetWindowAndConvertToUint8Image<char>(img, length, windowLevel, windowWidth);
}

unsigned char*
CircusCS_SetWindowAndConvertToUint8ImageFromUint16(unsigned short* img, int length, int windowLevel, int windowWidth)
{
	return SetWindowAndConvertToUint8Image<unsigned short>(img, length, windowLevel, windowWidth);
}

unsigned char*
CircusCS_SetWindowAndConvertToUint8ImageFromSint16(short* img, int length, int windowLevel, int windowWidth)
{
	return SetWindowAndConvertToUint8Image<short>(img, length, windowLevel, windowWidth);
}

unsigned char*
CircusCS_SetWindowAndConvertToUint8ImageFromUint32(unsigned int* img, int length, int windowLevel, int windowWidth)
{
	return SetWindowAndConvertToUint8Image<unsigned int>(img, length, windowLevel, windowWidth);
}

unsigned char*
CircusCS_SetWindowAndConvertToUint8ImageFromSint32(int* img, int length, int windowLevel, int windowWidth)
{
	return SetWindowAndConvertToUint8Image<int>(img, length, windowLevel, windowWidth);
}

