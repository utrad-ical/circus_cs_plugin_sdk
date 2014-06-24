////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			savePng.h : save PNG file (using VOL_RAWIMAGEDATA)  
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


#ifndef SAVE_PNG_H

#define COMPRESSION_LEVEL_MIN	1	// Z_BEST_SPEED
#define COMPRESSION_LEVEL_MAX	9	// Z_BEST_COMPRESSION

int CircusCS_SaveImageAsPng(
	char* fileName,
	unsigned char* img,
	int width,
	int height,
	int type=CircusCS_VALUETYPE_SINGLE,
	int compressLevel=COMPRESSION_LEVEL_MAX);

#endif

#define SAVE_PNG_H
