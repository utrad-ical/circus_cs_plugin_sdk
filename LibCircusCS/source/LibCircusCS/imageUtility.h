////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			imageUtility.h : utility for VOL_RAWIMAGEDATA 
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


#ifndef IMAGE_UTILITY_H

CircusCS_INTSIZE2D* CircusCS_GetImageSizeBySection(CircusCS_INTSIZE3D* matrix3D, int section);

template <typename VARTYPE> VARTYPE* CircusCS_ExtractSingleSliceFromVolumeData(
	VARTYPE* volume,
	CircusCS_INTSIZE3D* matrix3D,
	int sliceNum,
	int section,
	int type=CircusCS_VALUETYPE_SINGLE);

template <typename VARTYPE> unsigned char* CircusCS_SetWindowAndConvertToUint8Image(
	VARTYPE* img,
	int length,
	int windowLevel,
	int windowWidth);

#endif

#define IMAGE_UTILITY_H
