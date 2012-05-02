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
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "../LibCircusCS.h"

#include "imageUtility.private.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class VARTYPE> void
ExtractionSingleSlice(VARTYPE*** srcData, VARTYPE** dstData, VOL_INTSIZE2D* matrix2D, int n, int section)
{

	switch(section)
	{
		case AXIAL_SECTION: // Axial
			{
				for(int j=0; j<matrix2D->height; j++)
				for(int i=0; i<matrix2D->width;  i++)
				{
					dstData[j][i] = srcData[n][j][i];  
				}
				break;
			}
			
		case CORONAL_SECTION: // Coronal
			{
				for(int j=0; j<matrix2D->height; j++)
				for(int i=0; i<matrix2D->width;  i++)
				{
					dstData[j][i] = srcData[j][n][i];  
				}
				break;
			}
			
		case SAGITTAL_SECTION: // Sagital
			{
				for(int j=0; j<matrix2D->height; j++)
				for(int i=0; i<matrix2D->width;  i++)
				{
					dstData[j][i] = srcData[j][i][n];  
				}
				break;
			}
	}

	return;
}

template void ExtractionSingleSlice(unsigned char***  srcData, unsigned char**  dstData, VOL_INTSIZE2D* matrix2D, int n, int section);
template void ExtractionSingleSlice(char***           srcData, char**           dstData, VOL_INTSIZE2D* matrix2D, int n, int section);
template void ExtractionSingleSlice(unsigned short*** srcData, unsigned short** dstData, VOL_INTSIZE2D* matrix2D, int n, int section);
template void ExtractionSingleSlice(short***          srcData, short**          dstData, VOL_INTSIZE2D* matrix2D, int n, int section);
template void ExtractionSingleSlice(unsigned long***  srcData, unsigned long**  dstData, VOL_INTSIZE2D* matrix2D, int n, int section);
template void ExtractionSingleSlice(int***            srcData, int**            dstData, VOL_INTSIZE2D* matrix2D, int n, int section);

VOL_RAWIMAGEDATA*
CircusCS_ExtractSingleSliceFromRawVolumeData(VOL_RAWVOLUMEDATA* volume, int ch, int sliceNum, int section)
{
	
	if(ch < 0 || ch >= volume->matrixSize->channel)		return NULL;
	
	if(sliceNum < 0)	return NULL;

	VOL_RAWIMAGEDATA* ret = NULL;
	VOL_INTSIZE3D*    matrix3D = VOL_GetIntSize3DFromIntSize4D(volume->matrixSize);
	VOL_INTSIZE2D*    matrix2D = VOL_NewIntSize2D(0, 0);

	switch(section)
	{
		case AXIAL_SECTION:  // Axial
			if(sliceNum >= matrix3D->depth)		return NULL;
			matrix2D->width  = matrix3D->width;
			matrix2D->height = matrix3D->height;
			break;

		case CORONAL_SECTION:  // Coronal
			if(sliceNum >= matrix3D->height)	return NULL;
			matrix2D->width  = matrix3D->width;
			matrix2D->height = matrix3D->depth;
			break;
			
		case SAGITTAL_SECTION:  // Sagittal
			if(sliceNum >= matrix3D->width)	return NULL;
			matrix2D->width  = matrix3D->height;
			matrix2D->height = matrix3D->depth;
			break;
	} // end switch

	if((ret = VOL_NewSingleChannelRawImageData(matrix2D, volume->voxelUnit[ch], volume->voxelType[ch])) == NULL) return NULL;

	switch(volume->voxelUnit[ch])
	{
		case VOL_VALUEUNIT_UINT8:
			ExtractionSingleSlice((unsigned char***)volume->array4D[ch], (unsigned char**)ret->array3D[0],
							      matrix2D, sliceNum, section);
			break;

		case VOL_VALUEUNIT_SINT8:
			ExtractionSingleSlice((char***)volume->array4D[ch], (char**)ret->array3D[0],
							      matrix2D, sliceNum, section);
			break;

		case VOL_VALUEUNIT_UINT16:
			ExtractionSingleSlice((unsigned short***)volume->array4D[ch], (unsigned short**)ret->array3D[0],
							      matrix2D, sliceNum, section);
			break;

		case VOL_VALUEUNIT_SINT16:
			ExtractionSingleSlice((short***)volume->array4D[ch], (short**)ret->array3D[0],
							      matrix2D, sliceNum, section);
			break;

		case VOL_VALUEUNIT_UINT32:
			ExtractionSingleSlice((unsigned long***)volume->array4D[ch], (unsigned long**)ret->array3D[0],
							      matrix2D, sliceNum, section);
			break;

		case VOL_VALUEUNIT_SINT32:
			ExtractionSingleSlice((int***)volume->array4D[ch], (int**)ret->array3D[0],
							      matrix2D, sliceNum, section);
			break;

	}

	return ret;
}


int
CircusCS_SetWindowAndConvertToUint8Image(VOL_RAWIMAGEDATA* img, int ch, int windowLevel, int windowWidth)
{

	float min = (float)windowLevel - (float)windowWidth * 0.5f;
	float max = (float)windowLevel + (float)windowWidth * 0.5f;

	VOL_VALUERANGE* srcRange = VOL_NewValueRange(min, max);
	VOL_VALUERANGE* dstRange = VOL_GetValueRangeOfValueUnit(VOL_VALUEUNIT_UINT8);

	if(windowLevel == 0 && windowWidth == 0)
	{
		int length = VOL_GetNumberOfPixels(img);
		srcRange = VOL_GetValueRangeOfArray(img->data[ch], length, img->pixelUnit[ch]);
	}

	int ret = VOL_ConvertPixelUnit(img, ch, VOL_VALUEUNIT_UINT8, srcRange, dstRange,
		                           VOL_CONVERTUNIT_TYPE_SCALE_CHECKRANGE);

	return ret;
}


int
CircusCS_ResizeRawImageData(VOL_RAWIMAGEDATA* img, VOL_INTBOX2D* box, int backgroundType)
{
	int nDepth = img->matrixSize->depth;

	VOL_RAWVOLUMEDATA* rawVolumeData = NULL;
	VOL_RAWIMAGEDATA*  newData;
	VOL_INTSIZE4D      srcVolumeSize;
	VOL_INTBOX3D*      dstBox = VOL_NewIntBox3D(NULL, NULL);
	VOL_INTSIZE3D      newSize;

	srcVolumeSize.width   = img->matrixSize->width;
	srcVolumeSize.height  = img->matrixSize->height;
	srcVolumeSize.depth   = 1;
	srcVolumeSize.channel = img->matrixSize->depth;

	dstBox->origin->x    = box->origin->x;
	dstBox->origin->y    = box->origin->y;
	dstBox->origin->z    = 0;
	dstBox->size->width  = box->size->width;
	dstBox->size->height = box->size->height;
	dstBox->size->depth  = 1;

	newSize.width   = box->size->width;
	newSize.height  = box->size->height;
	newSize.depth   = img->matrixSize->depth;

	if((rawVolumeData = VOL_NewRawVolumeData(&srcVolumeSize, img->pixelUnit, img->pixelType)) == NULL) return -1;

	for(int i=0;i<nDepth;i++)	ConvertRawImageDataToRawVolumeData(img, i, rawVolumeData, i);

	VOL_ResizeRawVolumeData(rawVolumeData, dstBox, backgroundType);

	if((newData = VOL_NewRawImageData(&newSize, img->pixelUnit, img->pixelType)) == NULL) return -1;

	for(int i=0;i<nDepth;i++)	ConvertRawVolumeDataToRawImageData(rawVolumeData, i, newData, i);

	VOL_SwapContentsOfRawImageData(img, newData);
	VOL_DeleteRawVolumeData(rawVolumeData);
	VOL_DeleteRawImageData(newData);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
ConvertRawImageDataToRawVolumeData(VOL_RAWIMAGEDATA* srcData, int srcCh, VOL_RAWVOLUMEDATA* dstData, int dstCh)
{

	int length = VOL_GetNumberOfPixels(srcData);

	unsigned char	*dstPtr2Duc, *srcPtr2Duc;
	char			*dstPtr2Dsc, *srcPtr2Dsc;
	unsigned short	*dstPtr2Dus, *srcPtr2Dus;
	short			*dstPtr2Dss, *srcPtr2Dss;
	unsigned long	*dstPtr2Dul, *srcPtr2Dul;
	long			*dstPtr2Dsl, *srcPtr2Dsl;
	float			*dstPtr2Df,  *srcPtr2Df;
	double			*dstPtr2Dd,  *srcPtr2Dd;

	if( srcData->pixelUnit[srcCh] != dstData->voxelUnit[dstCh] )
	{
		return;
	}

	switch(srcData->pixelUnit[srcCh])
	{
		case VOL_VALUEUNIT_UINT8 :
			srcPtr2Duc = (unsigned char*)(srcData->data[srcCh]);
			dstPtr2Duc = (unsigned char*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Duc++ = *srcPtr2Duc++;
			break;
		case VOL_VALUEUNIT_SINT8 :
			srcPtr2Dsc = (char*)(srcData->data[srcCh]);
			dstPtr2Dsc = (char*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dsc++ = *srcPtr2Dsc++;
			break;
		case VOL_VALUEUNIT_UINT16 :
			srcPtr2Dus = (unsigned short*)(srcData->data[srcCh]);
			dstPtr2Dus = (unsigned short*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dus++ = *srcPtr2Dus++;
			break;
		case VOL_VALUEUNIT_SINT16 :
			srcPtr2Dss = (short*)(srcData->data[srcCh]);
			dstPtr2Dss = (short*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dss++ = *srcPtr2Dss++;
			break;
		case VOL_VALUEUNIT_UINT32 :
			srcPtr2Dul = (unsigned long*)(srcData->data[srcCh]);
			dstPtr2Dul = (unsigned long*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dul++ = *srcPtr2Dul++;
			break;
		case VOL_VALUEUNIT_SINT32 :
			srcPtr2Dsl = (long*)(srcData->data[srcCh]);
			dstPtr2Dsl = (long*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dsl++ = *srcPtr2Dsl++;
			break;
		case VOL_VALUEUNIT_FLOAT32 :
			srcPtr2Df = (float*)(srcData->data[srcCh]);
			dstPtr2Df = (float*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Df++ = *srcPtr2Df++;
			break;
		case VOL_VALUEUNIT_FLOAT64 :
			srcPtr2Dd = (double*)(srcData->data[srcCh]);
			dstPtr2Dd = (double*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dd++ = *srcPtr2Dd++;
			break;
	}
}



void
ConvertRawVolumeDataToRawImageData(VOL_RAWVOLUMEDATA* srcData, int srcCh, VOL_RAWIMAGEDATA* dstData, int dstCh)
{

	int length = VOL_GetNumberOfPixels(dstData);

	unsigned char	*dstPtr2Duc, *srcPtr2Duc;
	char			*dstPtr2Dsc, *srcPtr2Dsc;
	unsigned short	*dstPtr2Dus, *srcPtr2Dus;
	short			*dstPtr2Dss, *srcPtr2Dss;
	unsigned long	*dstPtr2Dul, *srcPtr2Dul;
	long			*dstPtr2Dsl, *srcPtr2Dsl;
	float			*dstPtr2Df,  *srcPtr2Df;
	double			*dstPtr2Dd,  *srcPtr2Dd;

	if( srcData->voxelUnit[srcCh] != dstData->pixelUnit[dstCh] )
	{
		return;
	}

	switch(srcData->voxelUnit[srcCh])
	{
		case VOL_VALUEUNIT_UINT8 :
			srcPtr2Duc = (unsigned char*)(srcData->data[srcCh]);
			dstPtr2Duc = (unsigned char*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Duc++ = *srcPtr2Duc++;
			break;
		case VOL_VALUEUNIT_SINT8 :
			srcPtr2Dsc = (char*)(srcData->data[srcCh]);
			dstPtr2Dsc = (char*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dsc++ = *srcPtr2Dsc++;
			break;
		case VOL_VALUEUNIT_UINT16 :
			srcPtr2Dus = (unsigned short*)(srcData->data[srcCh]);
			dstPtr2Dus = (unsigned short*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dus++ = *srcPtr2Dus++;
			break;
		case VOL_VALUEUNIT_SINT16 :
			srcPtr2Dss = (short*)(srcData->data[srcCh]);
			dstPtr2Dss = (short*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dss++ = *srcPtr2Dss++;
			break;
		case VOL_VALUEUNIT_UINT32 :
			srcPtr2Dul = (unsigned long*)(srcData->data[srcCh]);
			dstPtr2Dul = (unsigned long*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dul++ = *srcPtr2Dul++;
			break;
		case VOL_VALUEUNIT_SINT32 :
			srcPtr2Dsl = (long*)(srcData->data[srcCh]);
			dstPtr2Dsl = (long*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dsl++ = *srcPtr2Dsl++;
			break;
		case VOL_VALUEUNIT_FLOAT32 :
			srcPtr2Df = (float*)(srcData->data[srcCh]);
			dstPtr2Df = (float*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Df++ = *srcPtr2Df++;
			break;
		case VOL_VALUEUNIT_FLOAT64 :
			srcPtr2Dd = (double*)(srcData->data[srcCh]);
			dstPtr2Dd = (double*)(dstData->data[dstCh]);
			for(int i=0; i<length; i++)  *dstPtr2Dd++ = *srcPtr2Dd++;
			break;
	}
}