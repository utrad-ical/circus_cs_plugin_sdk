////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			saveJpeg.h : save JPEG file (using VOL_RAWIMAGEDATA) 
//
//			main coder: Y. Nomura
//
//			update history
//
//			2011.03.23	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef SAVE_JPEG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define JPEG_QUALITY_MIN	0
#define JPEG_QUALITY_MAX	100

int CircusCS_SaveJPEG(VOL_RAWIMAGEDATA* img, int ch, char* fileName,
					  int quality=JPEG_QUALITY_MAX);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

#define SAVE_JPEG_H
