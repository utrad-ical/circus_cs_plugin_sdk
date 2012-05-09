////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			LibCircusCS.h : main header file
//
//			main coder: Y. Nomura
//
//			update history
//
//			2011.03.23	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "LibCircusCS/basicPrimitive.h"
#include "LibCircusCS/defDicomTagStr.h"
#include "LibCircusCS/loadDicomDump.h"
#include "LibCircusCS/saveJpeg.h"
#include "LibCircusCS/savePng.h"
#include "LibCircusCS/rawVolumeIO.h"
#include "LibCircusCS/imageUtility.h"
#include "LibCircusCS/intensityProjection.h"

#ifdef WIN32
#include "LibCircusCS/winUtility.h"
#endif

#define MAX_STRING_LENGTH	2048

// Section
#define AXIAL_SECTION		0
#define CORONAL_SECTION		1
#define SAGITTAL_SECTION	2

// Image type
#define GLAY_SCALE		0
#define RGB_COLOR		1




