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


template <typename VARTYPE> VARTYPE*
	CircusCS_CreateIntensityProjection(VARTYPE* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section)
{
	switch(mode)
	{
	case MAX_INTENSITY:
		return CreateMaxIntensityProjection<VARTYPE>(volume, matrix3D, section);
		break;

	case MIN_INTENSITY:
		return CreateMinIntensityProjection<VARTYPE>(volume, matrix3D, section);
		break;

	case MEAN_INTENSITY:
		return CreateMeanIntensityProjection<VARTYPE>(volume, matrix3D, section);
		break;
	}
	return NULL;
}


template unsigned char*  CircusCS_CreateIntensityProjection<unsigned char>(unsigned char* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);
template char*           CircusCS_CreateIntensityProjection<char>(char* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);
template unsigned short* CircusCS_CreateIntensityProjection<unsigned short>(unsigned short* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);
template short*          CircusCS_CreateIntensityProjection<short>(short* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);
template unsigned int*   CircusCS_CreateIntensityProjection<unsigned int>(unsigned int* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);
template int*            CircusCS_CreateIntensityProjection<int>(int* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);
