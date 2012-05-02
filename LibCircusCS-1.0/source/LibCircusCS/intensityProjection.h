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
//
//
////////////////////////////////////////////////////////////////////////////////////////



#ifndef INTENSITY_PROJECTION_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// Projection mode
#define MAX_INTENSITY   0
#define MEAN_INTENSITY  1

VOL_RAWIMAGEDATA* CircusCS_CreateIntensityProjection(VOL_RAWVOLUMEDATA* volume, int ch, int mode, int section);

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif


#define INTENSITY_PROJECTION_H
