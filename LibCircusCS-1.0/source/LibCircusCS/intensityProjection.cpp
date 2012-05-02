////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			intensityProjection.cpp : for creating intensity projectionbimage
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

#include "intensityProjection.private.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class VARTYPE> VOL_RAWIMAGEDATA*
CalcMaxProjection(VARTYPE*** srcData, VOL_INTSIZE3D* matrix3D, int pixelUnit, int mode, int section)
{
	VOL_RAWIMAGEDATA* ret;
	VOL_INTSIZE2D*    matrix2D = VOL_NewIntSize2D(0, 0);

	switch(section)
	{
		case AXIAL_SECTION:  // Axial
			matrix2D->width = matrix3D->width;
			matrix2D->height = matrix3D->height;
			break;

		case CORONAL_SECTION:  // Coronal
			matrix2D->width = matrix3D->width;
			matrix2D->height = matrix3D->depth;
			break;
			
		case SAGITTAL_SECTION:  // Sagittal
			matrix2D->width = matrix3D->height;
			matrix2D->height = matrix3D->depth;
			break;
	} // end switch

	if((ret = VOL_NewSingleChannelRawImageData(matrix2D, pixelUnit, VOL_VALUETYPE_SINGLE)) == NULL) return NULL;

	VARTYPE** dstData = (VARTYPE**)ret->array3D[0];

	switch(section)
	{
		case AXIAL_SECTION: // Axial
			{
				for(int k=0; k<matrix3D->depth;  k++)
				for(int j=0; j<matrix3D->height; j++)
				for(int i=0; i<matrix3D->width;  i++)
				{
					if(k==0 || (k > 0 && dstData[j][i] < srcData[k][j][i])) dstData[j][i] = srcData[k][j][i];
				}
				break;
			}
			
		case CORONAL_SECTION: // Coronal
			{
				for(int j=0; j<matrix3D->height; j++)
				for(int k=0; k<matrix3D->depth;  k++)
				for(int i=0; i<matrix3D->width;  i++)
				{
					if(j==0 || (j > 0 && dstData[k][i] < srcData[k][j][i])) dstData[k][i] = srcData[k][j][i];  
				}
				break;
			}
			
		case SAGITTAL_SECTION: // Sagital
			{
				for(int i=0; i<matrix3D->width;  i++)
				for(int k=0; k<matrix3D->depth;  k++)
				for(int j=0; j<matrix3D->height; j++)
				{
					if(i==0 || (i > 0 && dstData[k][j] < srcData[k][j][i])) dstData[k][j] = srcData[k][j][i];
				}
				break;
			}
	} // end switch

	return ret;
}

template VOL_RAWIMAGEDATA* CalcMaxProjection(unsigned char***  srcData, VOL_INTSIZE3D* matrix3D, int pixelUnit, int mode, int section);
template VOL_RAWIMAGEDATA* CalcMaxProjection(char***           srcData, VOL_INTSIZE3D* matrix3D, int pixelUnit, int mode, int section);
template VOL_RAWIMAGEDATA* CalcMaxProjection(unsigned short*** srcData, VOL_INTSIZE3D* matrix3D, int pixelUnit, int mode, int section);
template VOL_RAWIMAGEDATA* CalcMaxProjection(short***          srcData, VOL_INTSIZE3D* matrix3D, int pixelUnit, int mode, int section);


template <class VARTYPE> VOL_RAWIMAGEDATA*
CalcMeanProjection(VARTYPE*** srcData, VOL_INTSIZE3D* matrix3D, int pixelUnit, int mode, int section)
{
	VOL_RAWIMAGEDATA* ret;
	VOL_RAWIMAGEDATA* tmpImg;

	int		          dstDepth = 0;
	VOL_INTSIZE2D*    matrix2D = VOL_NewIntSize2D(0, 0);

	switch(section)
	{
		case AXIAL_SECTION:  // Axial
			matrix2D->width = matrix3D->width;
			matrix2D->height = matrix3D->height;
			dstDepth = matrix3D->depth;
			break;

		case CORONAL_SECTION:  // Coronal
			matrix2D->width = matrix3D->width;
			matrix2D->height = matrix3D->depth;
			dstDepth = matrix3D->height;
			break;
			
		case SAGITTAL_SECTION:  // Sagittal
			matrix2D->width = matrix3D->height;
			matrix2D->height = matrix3D->depth;
			dstDepth = matrix3D->width;
			break;
	} // end switch

	if((ret = VOL_NewSingleChannelRawImageData(matrix2D, pixelUnit, VOL_VALUETYPE_SINGLE)) == NULL) return NULL;

	if((tmpImg = VOL_NewSingleChannelRawImageData(matrix2D, VOL_VALUEUNIT_SINT32, VOL_VALUETYPE_SINGLE)) == NULL)
	{
		VOL_DeleteRawImageData(ret);	
		return NULL;
	}

	int**     tmpData = (int**)tmpImg->array3D[0];
	VARTYPE** dstData = (VARTYPE**)ret->array3D[0];

	switch(section)
	{
		case AXIAL_SECTION: // Axial
			{
				for(int k=0; k<matrix3D->depth;  k++)
				for(int j=0; j<matrix3D->height; j++)
				for(int i=0; i<matrix3D->width;  i++)
				{
					tmpData[j][i] += srcData[k][j][i];  
				}
				break;
			}
			
		case CORONAL_SECTION: // Coronal
			{
				for(int j=0; j<matrix3D->height; j++)
				for(int k=0; k<matrix3D->depth;  k++)
				for(int i=0; i<matrix3D->width;  i++)
				{
					tmpData[k][i] += srcData[k][j][i];  
				}
				break;
			}
			
		case SAGITTAL_SECTION: // Sagital
			{
				for(int i=0; i<matrix3D->width;  i++)
				for(int k=0; k<matrix3D->depth;  k++)
				for(int j=0; j<matrix3D->height; j++)
				{
					tmpData[k][j] += srcData[k][j][i];  
				}
				break;
			}
	} // end switch

	for(int j=0; j<ret->matrixSize->height; j++)
	for(int i=0; i<ret->matrixSize->width;  i++)
	{
		dstData[j][i] = (VARTYPE)((double)tmpData[j][i] / dstDepth);
	}

	VOL_DeleteRawImageData(tmpImg);

	return ret;
}

template VOL_RAWIMAGEDATA* CalcMeanProjection(unsigned char***  srcData, VOL_INTSIZE3D* matrix3D, int pixelUnit, int mode, int section);
template VOL_RAWIMAGEDATA* CalcMeanProjection(char***           srcData, VOL_INTSIZE3D* matrix3D, int pixelUnit, int mode, int section);
template VOL_RAWIMAGEDATA* CalcMeanProjection(unsigned short*** srcData, VOL_INTSIZE3D* matrix3D, int pixelUnit, int mode, int section);
template VOL_RAWIMAGEDATA* CalcMeanProjection(short***          srcData, VOL_INTSIZE3D* matrix3D, int pixelUnit, int mode, int section);



VOL_RAWIMAGEDATA*
CircusCS_CreateIntensityProjection(VOL_RAWVOLUMEDATA* volume, int ch, int mode, int section)
{
	if(ch < 0 || ch >= volume->matrixSize->channel)		return NULL;

	VOL_RAWIMAGEDATA* ret = NULL;
	VOL_INTSIZE3D* matrix3D = VOL_GetIntSize3DFromIntSize4D(volume->matrixSize);

	int pixelUnit = volume->voxelUnit[ch];

	if(mode == MAX_INTENSITY)
	{
		switch(pixelUnit)
		{
			case VOL_VALUEUNIT_UINT8:  ret = CalcMaxProjection((unsigned char***)volume->array4D[ch],  matrix3D, pixelUnit, mode, section); break;
			case VOL_VALUEUNIT_SINT8:  ret = CalcMaxProjection((char***)volume->array4D[ch],           matrix3D, pixelUnit, mode, section); break;
			case VOL_VALUEUNIT_UINT16: ret = CalcMaxProjection((unsigned short***)volume->array4D[ch], matrix3D, pixelUnit, mode, section); break;
			case VOL_VALUEUNIT_SINT16: ret = CalcMaxProjection((short***)volume->array4D[ch],          matrix3D, pixelUnit, mode, section); break;
		}
	}
	
	if(mode == MEAN_INTENSITY)
	{
		switch(pixelUnit)
		{
			case VOL_VALUEUNIT_UINT8:  ret = CalcMeanProjection((unsigned char***)volume->array4D[ch],  matrix3D, pixelUnit, mode, section); break;
			case VOL_VALUEUNIT_SINT8:  ret = CalcMeanProjection((char***)volume->array4D[ch],           matrix3D, pixelUnit, mode, section); break;
			case VOL_VALUEUNIT_UINT16: ret = CalcMeanProjection((unsigned short***)volume->array4D[ch], matrix3D, pixelUnit, mode, section); break;
			case VOL_VALUEUNIT_SINT16: ret = CalcMeanProjection((short***)volume->array4D[ch],          matrix3D, pixelUnit, mode, section); break;
		}
	}

	return ret;
}


