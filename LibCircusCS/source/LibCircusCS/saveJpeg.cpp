////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			saveJpeg.cpp : save JPEG file (using VOL_RAWIMAGEDATA) 
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

#define _CRT_SECURE_NO_DEPRECATE 

#include "../LibCircusCS.h"

#include "saveJpeg.private.h"

#include "jpeglib.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CircusCS_SaveImageAsJpeg(
	char* fileName,
	unsigned char* img,
	int width,
	int height,
	int type,
	int quality)
{
	if(quality < JPEG_QUALITY_MIN || quality > JPEG_QUALITY_MAX)  quality = JPEG_QUALITY_MAX;

	//----------------------------------------------------------------------------------------------
	// Open file
	//----------------------------------------------------------------------------------------------
	FILE* fp = fopen(fileName, "wb");
	if(!fp)
	{
		fprintf(stderr, "Failed to open file: %s\n", fileName);
		return -1;
	}
	//----------------------------------------------------------------------------------------------

	jpeg_compress_struct jpegStruct;    // Structure for JPEG compression
	jpeg_error_mgr jpegErr;				// Structure for error process

	jpegStruct.err = jpeg_std_error(&jpegErr);
	jpeg_create_compress(&jpegStruct);

	jpeg_stdio_dest(&jpegStruct, fp);

	jpegStruct.image_width  = width;
	jpegStruct.image_height = height;
	jpegStruct.input_components = 3;
	jpegStruct.in_color_space = JCS_RGB;
	jpeg_set_defaults(&jpegStruct);
	jpeg_set_quality(&jpegStruct, quality, true);

	jpeg_start_compress(&jpegStruct, true);

	JSAMPLE* buffer = new JSAMPLE[width * 3];

	//unsigned long** srcData = (unsigned long**)tmpImg->array3D[ch];
	int j=0;

	while(jpegStruct.next_scanline < jpegStruct.image_height)
	{
		JSAMPLE* bufPtr = buffer;

		for(int i=0; i<width; i++)
		{
			if(type == CircusCS_VALUETYPE_RGB)
			{
				int pos = (j * width + i) * 3;	

				*bufPtr++ = (JSAMPLE)img[pos];		// red
				*bufPtr++ =	(JSAMPLE)img[pos + 1];	// green
				*bufPtr++ = (JSAMPLE)img[pos + 2];	// blue
			}
			else  // PIXEL_TYPE_GLAYSCALE
			{
				unsigned char pixelVal = img[j * width + i];	

				*bufPtr++ = (JSAMPLE)pixelVal;		// red
				*bufPtr++ =	(JSAMPLE)pixelVal;		// green
				*bufPtr++ = (JSAMPLE)pixelVal;		// blue
			}
		}
		jpeg_write_scanlines( &jpegStruct, &buffer, 1 );
		j++;
	}

	jpeg_finish_compress( &jpegStruct );

	fclose(fp);

	delete [] buffer;

	jpeg_destroy_compress( &jpegStruct );

	return 0;
}
