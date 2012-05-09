////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			intensityProjection.h : for creating intensity projectionbimage
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

#ifndef INTENSITY_PROJECTION_H

// Projection mode
#define MAX_INTENSITY   0
#define MIN_INTENSITY   1
#define MEAN_INTENSITY  2

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

unsigned char*  CircusCS_CreateIntensityProjectionAsUint8(unsigned char* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);
char*           CircusCS_CreateIntensityProjectionAsSint8(char* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);
unsigned short* CircusCS_CreateIntensityProjectionAsUint16(unsigned short* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);
short*          CircusCS_CreateIntensityProjectionAsSint16(short* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);
unsigned int*   CircusCS_CreateIntensityProjectionAsUint32(unsigned int* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);
int*            CircusCS_CreateIntensityProjectionAsSint32(int* volume, CircusCS_INTSIZE3D* matrix3D, int mode, int section);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

#define INTENSITY_PROJECTION_H
