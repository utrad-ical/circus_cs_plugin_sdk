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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CircusCS_INTSIZE2D* CircusCS_GetImageSizeBySection(CircusCS_INTSIZE3D* matrix3D, int section);

unsigned char*  CircusCS_ExtractSingleSliceFromVolumeDataAsUint8(unsigned char* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section);
char*           CircusCS_ExtractSingleSliceFromVolumeDataAsSint8(char* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section);
unsigned short* CircusCS_ExtractSingleSliceFromVolumeDataAsUint16(unsigned short* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section);
short*          CircusCS_ExtractSingleSliceFromVolumeDataAsSint16(short* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section);
unsigned int*   CircusCS_ExtractSingleSliceFromVolumeDataAsUint32(unsigned int* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section);
int*            CircusCS_ExtractSingleSliceFromVolumeDataAsSint32(int* volume, CircusCS_INTSIZE3D* matrix3D, int sliceNum, int section);

unsigned char*  CircusCS_SetWindowAndConvertToUint8ImageFromUint8(unsigned char* img, int length, int windowLevel, int windowWidth);
unsigned char*  CircusCS_SetWindowAndConvertToUint8ImageFromSint8(char* img, int length, int windowLevel, int windowWidth);
unsigned char*  CircusCS_SetWindowAndConvertToUint8ImageFromUint16(unsigned short* img, int length, int windowLevel, int windowWidth);
unsigned char*  CircusCS_SetWindowAndConvertToUint8ImageFromSint16(short* img, int length, int windowLevel, int windowWidth);
unsigned char*  CircusCS_SetWindowAndConvertToUint8ImageFromUint32(unsigned int* img, int length, int windowLevel, int windowWidth);
unsigned char*  CircusCS_SetWindowAndConvertToUint8ImageFromSint32(int* img, int length, int windowLevel, int windowWidth);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

#define IMAGE_UTILITY_H
