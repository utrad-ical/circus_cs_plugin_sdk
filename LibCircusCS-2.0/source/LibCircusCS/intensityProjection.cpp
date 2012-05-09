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
//			2012.05.08  rewrite without VOL library
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "../LibCircusCS.h"

#include "intensityProjection.private.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename VARTYPE> VARTYPE*
CreateMaxIntensityProjection(VARTYPE* volume, CircusCS_INTSIZE3D* matrix3D, int section)
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
					ret[pos2D] = volume[pos2D]; 
		
					for(int k=1; k<matrix3D->depth; k++)
					{
						pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width + i;
						if(ret[pos2D] < volume[pos3D])  ret[pos2D] = volume[pos3D];
					}
				}
				break;
			}
			
		case CORONAL_SECTION: // Coronal
			{
				for(int k=0; k<matrix3D->depth; k++)
				for(int i=0; i<matrix3D->width; i++)
				{
					pos2D = k*matrix2D->width + i;
					pos3D = k*matrix3D->height*matrix3D->width + i;
					ret[pos2D] = volume[pos3D]; 

					for(int j=1; j<matrix3D->height; j++)
					{
						pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width + i;
						if(ret[pos2D] < volume[pos3D])  ret[pos2D] = volume[pos3D];
					}
				}
				break;
			}

		case SAGITTAL_SECTION: // Sagital
			{
				for(int k=0; k<matrix3D->depth;  k++)
				for(int j=0; j<matrix3D->height; j++)
				{
					pos2D = k*matrix2D->width + j;
					pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width;
					ret[pos2D] = volume[pos3D]; 

					for(int i=1; i<matrix3D->width; i++)
					{
						pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width + i;
						if(ret[pos2D] < volume[pos3D])  ret[pos2D] = volume[pos3D];
					}
				}
				break;
			}
	} // end switch

	return ret;
}


template <typename VARTYPE> VARTYPE*
CreateMinIntensityProjection(VARTYPE* volume, CircusCS_INTSIZE3D* matrix3D, int section)
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
					ret[pos2D] = volume[pos2D]; 
		
					for(int k=1; k<matrix3D->depth; k++)
					{
						pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width + i;
						if(ret[pos2D] > volume[pos3D])  ret[pos2D] = volume[pos3D];
					}
				}
				break;
			}
			
		case CORONAL_SECTION: // Coronal
			{
				for(int k=0; k<matrix3D->depth; k++)
				for(int i=0; i<matrix3D->width; i++)
				{
					pos2D = k*matrix2D->width + i;
					pos3D = k*matrix3D->height*matrix3D->width + i;
					ret[pos2D] = volume[pos3D]; 

					for(int j=1; j<matrix3D->height; j++)
					{
						pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width + i;
						if(ret[pos2D] > volume[pos3D])  ret[pos2D] = volume[pos3D];
					}
				}
				break;
			}

		case SAGITTAL_SECTION: // Sagital
			{
				for(int k=0; k<matrix3D->depth;  k++)
				for(int j=0; j<matrix3D->height; j++)
				{
					pos2D = k*matrix2D->width + j;
					pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width;
					ret[pos2D] = volume[pos3D]; 

					for(int i=1; i<matrix3D->width; i++)
					{
						pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width + i;
						if(ret[pos2D] > volume[pos3D])  ret[pos2D] = volume[pos3D];
					}
				}
				break;
			}
	} // end switch

	return ret;
}


template <typename VARTYPE> VARTYPE*
CreateMeanIntensityProjection(VARTYPE* volume, CircusCS_INTSIZE3D* matrix3D, int section)
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
					double tmp = 0;
					
					for(int k=0; k<matrix3D->depth; k++)
					{
						pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width + i;
						tmp += volume[pos3D];
					}
					ret[pos2D] = (VARTYPE)(tmp / matrix3D->depth + 0.5);
				}
				break;
			}
			
		case CORONAL_SECTION: // Coronal
			{
				for(int k=0; k<matrix3D->depth; k++)
				for(int i=0; i<matrix3D->width; i++)
				{
					pos2D = k*matrix2D->width + i;
					double tmp = 0;

					for(int j=0; j<matrix3D->height; j++)
					{
						pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width + i;
						tmp += volume[pos3D];
					}
					ret[pos2D] = (VARTYPE)(tmp / matrix3D->height + 0.5);
				}
				break;
			}

		case SAGITTAL_SECTION: // Sagital
			{
				for(int k=0; k<matrix3D->depth;  k++)
				for(int j=0; j<matrix3D->height; j++)
				{
					pos2D = k*matrix2D->width + j;
					double tmp = 0;

					for(int i=0; i<matrix3D->width; i++)
					{
						pos3D = k*matrix3D->height*matrix3D->width + j*matrix3D->width + i;
						tmp += volume[pos3D];
					}
					ret[pos2D] = (VARTYPE)(tmp / matrix3D->width + 0.5);
				}
				break;
			}
	} // end switch

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char*
CircusCS_CreateIntensityProjectionAsUint8(unsigned char* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section)
{
	switch(mode)
	{
		case MAX_INTENSITY:
				return CreateMaxIntensityProjection<unsigned char>(volume, matrix3D, section);
				break;
		
		case MIN_INTENSITY:
				return CreateMinIntensityProjection<unsigned char>(volume, matrix3D, section);
				break;

		case MEAN_INTENSITY:
				return CreateMeanIntensityProjection<unsigned char>(volume, matrix3D, section);
				break;
	}
	return NULL;
}
					
char*
CircusCS_CreateIntensityProjectionAsSint8(char* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section)
{
	switch(mode)
	{
		case MAX_INTENSITY:
				return CreateMaxIntensityProjection<char>(volume, matrix3D, section);
				break;
		
		case MIN_INTENSITY:
				return CreateMinIntensityProjection<char>(volume, matrix3D, section);
				break;

		case MEAN_INTENSITY:
				return CreateMeanIntensityProjection<char>(volume, matrix3D, section);
				break;
	}
	return NULL;
}


unsigned short*
CircusCS_CreateIntensityProjectionAsUint16(unsigned short* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section)
{
	switch(mode)
	{
		case MAX_INTENSITY:
				return CreateMaxIntensityProjection<unsigned short>(volume, matrix3D, section);
				break;
		
		case MIN_INTENSITY:
				return CreateMinIntensityProjection<unsigned short>(volume, matrix3D, section);
				break;

		case MEAN_INTENSITY:
				return CreateMeanIntensityProjection<unsigned short>(volume, matrix3D, section);
				break;
	}
	return NULL;
}
					
short*
CircusCS_CreateIntensityProjectionAsSint16(short* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section)
{
	switch(mode)
	{
		case MAX_INTENSITY:
				return CreateMaxIntensityProjection<short>(volume, matrix3D, section);
				break;
		
		case MIN_INTENSITY:
				return CreateMinIntensityProjection<short>(volume, matrix3D, section);
				break;

		case MEAN_INTENSITY:
				return CreateMeanIntensityProjection<short>(volume, matrix3D, section);
				break;
	}
	return NULL;
}

unsigned int*
CircusCS_CreateIntensityProjectionAsUint32(unsigned int* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section)
{
	switch(mode)
	{
		case MAX_INTENSITY:
				return CreateMaxIntensityProjection<unsigned int>(volume, matrix3D, section);
				break;
		
		case MIN_INTENSITY:
				return CreateMinIntensityProjection<unsigned int>(volume, matrix3D, section);
				break;

		case MEAN_INTENSITY:
				return CreateMeanIntensityProjection<unsigned int>(volume, matrix3D, section);
				break;
	}
	return NULL;
}
					
int*
CircusCS_CreateIntensityProjectionAsSint32(int* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section)
{
	switch(mode)
	{
		case MAX_INTENSITY:
				return CreateMaxIntensityProjection<int>(volume, matrix3D, section);
				break;
		
		case MIN_INTENSITY:
				return CreateMinIntensityProjection<int>(volume, matrix3D, section);
				break;

		case MEAN_INTENSITY:
				return CreateMeanIntensityProjection<int>(volume, matrix3D, section);
				break;
	}
	return NULL;
}
