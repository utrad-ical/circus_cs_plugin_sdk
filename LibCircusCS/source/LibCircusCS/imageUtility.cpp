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

CircusCS_INTSIZE2D* CircusCS_GetImageSizeBySection(CircusCS_INTSIZE3D* matrix3D, int section)
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
	CircusCS_ExtractSingleSliceFromVolumeData(
	VARTYPE* volume,
	CircusCS_INTSIZE3D* matrix3D,
	int sliceNum,
	int section,
	int type)
{
	// Check sliceNum
	if(sliceNum < 0
		|| (section == AXIAL_SECTION    && sliceNum >= matrix3D->depth)
		|| (section == CORONAL_SECTION  && sliceNum >= matrix3D->height)
		|| (section == SAGITTAL_SECTION && sliceNum >= matrix3D->width))
	{
		fprintf(stderr, "sliceNum is incorrect.\n");
		return NULL;
	}

	VARTYPE*            ret = NULL;
	CircusCS_INTSIZE2D* matrix2D = CircusCS_GetImageSizeBySection(matrix3D, section);

	int size = matrix2D->width * matrix2D->height;
	if(type == CircusCS_VALUETYPE_RGB)  size *= 3;

	if((ret = (VARTYPE*)calloc(size, sizeof(VARTYPE))) == NULL) return NULL;

	int pos2D, pos3D;

	switch(section)
	{
	case AXIAL_SECTION:
		{
			for(int j=0; j<matrix3D->height; j++)
				for(int i=0; i<matrix3D->width;  i++)
				{
					pos2D = j*matrix2D->width + i;
					pos3D = sliceNum*matrix3D->height*matrix3D->width + j*matrix3D->width + i;

					if(type == CircusCS_VALUETYPE_RGB)
					{
						ret[pos2D * 3]     = volume[pos3D * 3];
						ret[pos2D * 3 + 1] = volume[pos3D * 3 + 1];
						ret[pos2D * 3 + 2] = volume[pos3D * 3 + 2];
					}
					else  // PIXEL_TYPE_GLAYSCALE
					{
						ret[pos2D] = volume[pos3D];
					}
				}
				break;
		}

	case CORONAL_SECTION:
		{
			for(int k=0; k<matrix3D->depth; k++)
				for(int i=0; i<matrix3D->width; i++)
				{
					pos2D = k*matrix2D->width + i;
					pos3D = k*matrix3D->height*matrix3D->width + sliceNum*matrix3D->width + i;

					if(type == CircusCS_VALUETYPE_RGB)
					{
						ret[pos2D * 3]     = volume[pos3D * 3];
						ret[pos2D * 3 + 1] = volume[pos3D * 3 + 1];
						ret[pos2D * 3 + 2] = volume[pos3D * 3 + 2];
					}
					else  // PIXEL_TYPE_GLAYSCALE
					{
						ret[pos2D] = volume[pos3D];
					}
				}
				break;
		}

	case SAGITTAL_SECTION:
		{
			for(int k=0; k<matrix3D->depth;  k++)
				for(int j=0; j<matrix3D->height; j++)
				{
					pos2D = k*matrix2D->width + j;
					pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width + sliceNum;

					if(type == CircusCS_VALUETYPE_RGB)
					{
						ret[pos2D * 3]     = volume[pos3D * 3];
						ret[pos2D * 3 + 1] = volume[pos3D * 3 + 1];
						ret[pos2D * 3 + 2] = volume[pos3D * 3 + 2];
					}
					else  // PIXEL_TYPE_GLAYSCALE
					{
						ret[pos2D] = volume[pos3D];
					}
				}
				break;
		}
	} // end switch

	return ret;
}

template unsigned char*  CircusCS_ExtractSingleSliceFromVolumeData<unsigned char>(unsigned char* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section, int type);
template char*           CircusCS_ExtractSingleSliceFromVolumeData<char>(char* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section, int type);
template unsigned short* CircusCS_ExtractSingleSliceFromVolumeData<unsigned short>(unsigned short* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section, int type);
template short*          CircusCS_ExtractSingleSliceFromVolumeData<short>(short* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section, int type);
template unsigned int*   CircusCS_ExtractSingleSliceFromVolumeData<unsigned int>(unsigned int* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section, int type);
template int*            CircusCS_ExtractSingleSliceFromVolumeData<int>(int* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section, int type);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename VARTYPE> unsigned char* 
	CircusCS_SetWindowAndConvertToUint8Image(VARTYPE* img, int length, int windowLevel, int windowWidth)
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
		else if(img[i] > max)  ret[i] = 255;
		else
		{
			double tmp = 255.0 * (img[i] - min) / (max - min); 
			ret[i] = (unsigned char)(tmp + 0.5);
		}
	}

	return ret;
}

template unsigned char* CircusCS_SetWindowAndConvertToUint8Image<unsigned char>(unsigned char* img, int length, int windowLevel, int windowWidth);
template unsigned char* CircusCS_SetWindowAndConvertToUint8Image<char>(char* img, int length, int windowLevel, int windowWidth);
template unsigned char* CircusCS_SetWindowAndConvertToUint8Image<unsigned short>(unsigned short* img, int length, int windowLevel, int windowWidth);
template unsigned char* CircusCS_SetWindowAndConvertToUint8Image<short>(short* img, int length, int windowLevel, int windowWidth);
template unsigned char* CircusCS_SetWindowAndConvertToUint8Image<unsigned int>(unsigned int* img, int length, int windowLevel, int windowWidth);
template unsigned char* CircusCS_SetWindowAndConvertToUint8Image<int>(int* img, int length, int windowLevel, int windowWidth);
