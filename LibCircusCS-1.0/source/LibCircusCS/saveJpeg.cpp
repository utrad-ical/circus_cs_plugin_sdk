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
//
//
////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE 

#include "../LibCircusCS.h"

#include "saveJpeg.private.h"

#include "jpeglib.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int
CircusCS_SaveJPEG(VOL_RAWIMAGEDATA* img, int ch, char* fileName, int quality)
{

	if(quality < JPEG_QUALITY_MIN || quality > JPEG_QUALITY_MAX)  quality = JPEG_QUALITY_MAX;

	if(!((img->pixelType[ch] == VOL_VALUETYPE_RGBA)
		 || (img->pixelType[ch] == VOL_VALUETYPE_SINGLE && img->pixelUnit[ch] == VOL_VALUEUNIT_UINT8)))
	{
		return -1;
	}

	VOL_RAWIMAGEDATA* tmpImg = VOL_DuplicateRawImageData(img);

	if(img->pixelType[ch] == VOL_VALUETYPE_SINGLE && img->pixelUnit[ch] == VOL_VALUEUNIT_UINT8)
	{
		VOL_DuplicateChannelOfRawImageData(tmpImg, ch);
		VOL_DuplicateChannelOfRawImageData(tmpImg, ch);

		// Set alpha channel
		VOL_AddNewChannelInRawImageData(tmpImg, VOL_VALUEUNIT_UINT8, VOL_VALUETYPE_SINGLE);

		VOL_ChangeColorTypeOfRgbaRawImageData(tmpImg);
	}

	FILE* fp = fopen(fileName, "wb");
	if(!fp)		return -1;

	jpeg_compress_struct jpegStruct;    // Structure for JPEG compression
	jpeg_error_mgr jpegErr;				// Structure for error process

	jpegStruct.err = jpeg_std_error(&jpegErr);
	jpeg_create_compress(&jpegStruct);

	jpeg_stdio_dest(&jpegStruct, fp);

	jpegStruct.image_width  = tmpImg->matrixSize->width;
	jpegStruct.image_height = tmpImg->matrixSize->height;
	jpegStruct.input_components = 3;
	jpegStruct.in_color_space = JCS_RGB;
	jpeg_set_defaults(&jpegStruct);
	jpeg_set_quality(&jpegStruct, quality, true);

	jpeg_start_compress(&jpegStruct, true);

	JSAMPLE* buffer = new JSAMPLE[ tmpImg->matrixSize->width * 3 ];

	unsigned long** srcData = (unsigned long**)tmpImg->array3D[ch];
	int j=0;

	while(jpegStruct.next_scanline < jpegStruct.image_height)
	{
		JSAMPLE* bufPtr = buffer;

		for(int i=0; i<tmpImg->matrixSize->width;  i++)
		{
			unsigned long pixelVal = srcData[j][i];	

			*bufPtr++ = (JSAMPLE)((unsigned char)((pixelVal >> 24) % 256));		// red
			*bufPtr++ =	(JSAMPLE)((unsigned char)((pixelVal >> 16) % 256));		// green
			*bufPtr++ = (JSAMPLE)((unsigned char)((pixelVal >> 8) % 256));		// blue
		}
		jpeg_write_scanlines( &jpegStruct, &buffer, 1 );
		j++;
	}

	jpeg_finish_compress( &jpegStruct );
	
	fclose(fp);

	delete [] buffer;

	jpeg_destroy_compress( &jpegStruct );

	VOL_DeleteRawImageData(tmpImg);

	return 0;
}
