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

template <typename VARTYPE> VARTYPE* CircusCS_CreateIntensityProjection(
	VARTYPE* volume,
	CircusCS_INTSIZE3D* matrix3D,
	int mode,
	int section);

#endif

#define INTENSITY_PROJECTION_H
