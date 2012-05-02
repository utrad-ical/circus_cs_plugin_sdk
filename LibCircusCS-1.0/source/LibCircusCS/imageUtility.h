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
//
//
////////////////////////////////////////////////////////////////////////////////////////



#ifndef IMAGE_UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

VOL_RAWIMAGEDATA* CircusCS_ExtractSingleSliceFromRawVolumeData(VOL_RAWVOLUMEDATA* volume, int ch, int sliceNum, int section);

int CircusCS_SetWindowAndConvertToUint8Image(VOL_RAWIMAGEDATA* img, int ch, int windowLevel, int windowWidth);

int CircusCS_ResizeRawImageData(VOL_RAWIMAGEDATA* img, VOL_INTBOX2D* box, int backgroundType);


#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif


#define IMAGE_UTILITY_H
