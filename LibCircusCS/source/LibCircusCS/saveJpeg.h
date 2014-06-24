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
//			2012.05.08  rewrite without VOL library
//
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef SAVE_JPEG_H

#define JPEG_QUALITY_MIN	0
#define JPEG_QUALITY_MAX	100

int CircusCS_SaveImageAsJpeg(
	char* fileName,
	unsigned char* img,
	int width,
	int height,
	int type=CircusCS_VALUETYPE_SINGLE,
	int quality=JPEG_QUALITY_MAX);

#endif

#define SAVE_JPEG_H
