////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			savePng.cpp : save PNG file (using VOL_RAWIMAGEDATA)  
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

#include "savePng.private.h"

#include "png.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int
CircusCS_SavePNG(VOL_RAWIMAGEDATA* img, int ch, char* fileName, int compressLevel)
{
	if(!((img->pixelType[ch] == VOL_VALUETYPE_RGBA)
		 || (img->pixelType[ch] == VOL_VALUETYPE_SINGLE && img->pixelUnit[ch] == VOL_VALUEUNIT_UINT8)))
	{
		return -1;
	}

	VOL_RAWIMAGEDATA* tmpImg = VOL_DuplicateRawImageData(img);

	//----------------------------------------------------------------------------------------------
	// Convert single image data to RGBA image data 
	//----------------------------------------------------------------------------------------------
	if(img->pixelType[ch] == VOL_VALUETYPE_SINGLE && img->pixelUnit[ch] == VOL_VALUEUNIT_UINT8)
	{
		VOL_DuplicateChannelOfRawImageData(tmpImg, ch);
		VOL_DuplicateChannelOfRawImageData(tmpImg, ch);

		// Set alpha channel (dummy)
		VOL_AddNewChannelInRawImageData(tmpImg, VOL_VALUEUNIT_UINT8, VOL_VALUETYPE_SINGLE);
		
		int pixelNum  = VOL_GetNumberOfPixels(img);
		unsigned char* alphaPtr = (unsigned char*)tmpImg->data[3];

		for(int i=0; i<pixelNum; i++)
		{
			*alphaPtr++ = (unsigned char)255;
		}

		VOL_ChangeColorTypeOfRgbaRawImageData(tmpImg);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// Open file
	//----------------------------------------------------------------------------------------------
	FILE* fp = fopen(fileName, "wb");
	if(!fp)		return -1;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// initialize png_struct and png_info
	//----------------------------------------------------------------------------------------------
	png_struct* pngStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!pngStruct)
	{
		fclose(fp);
		return -1;
	}

	png_info* pngInfo = png_create_info_struct(pngStruct);
	if (!pngInfo)
	{
		png_destroy_write_struct(&pngStruct, NULL);
		fclose(fp);
		return -1;
	}

	if(setjmp(pngStruct->jmpbuf))
	{
		png_destroy_write_struct(&pngStruct, &pngInfo);
		fclose(fp);
		return -1;
	}

	// set file pointer
	png_init_io(pngStruct, fp);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// set parameters
	//----------------------------------------------------------------------------------------------
	if(compressLevel < COMPRESSION_LEVEL_MIN || compressLevel > COMPRESSION_LEVEL_MAX)
	{
		png_set_compression_level(pngStruct, Z_BEST_COMPRESSION );
	}
	else
	{
		png_set_compression_level(pngStruct, compressLevel);
	}

	int pngColorType = PNG_COLOR_TYPE_RGB;

	png_set_IHDR(pngStruct, pngInfo,
				 tmpImg->matrixSize->width, tmpImg->matrixSize->height,
				 8, pngColorType, PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// set pixel data
	//----------------------------------------------------------------------------------------------
	png_bytepp pngBuff;
	pngBuff = new png_byte*[tmpImg->matrixSize->height];

	int chNum = 3;

	unsigned long** srcData = (unsigned long**)tmpImg->array3D[ch];

	for(int j=0; j<tmpImg->matrixSize->height; j++)
	{
		pngBuff[j] = new png_byte[tmpImg->matrixSize->width * chNum];

		for(int i=0; i<tmpImg->matrixSize->width; i++)
		{
			unsigned long pixelVal = srcData[j][i];
			
			pngBuff[j][i * chNum + 0] = (unsigned char)((pixelVal >> 24) % 256);	// red
			pngBuff[j][i * chNum + 1] = (unsigned char)((pixelVal >> 16) % 256);	// green
			pngBuff[j][i * chNum + 2] = (unsigned char)((pixelVal >> 8)  % 256);	// blue
		}
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// write png_info and pixel data
	//----------------------------------------------------------------------------------------------
	png_write_info(pngStruct, pngInfo);
    png_write_image(pngStruct, pngBuff);
	png_write_end(pngStruct, NULL );
	png_destroy_write_struct(&pngStruct, &pngInfo);
	fclose(fp);
	//----------------------------------------------------------------------------------------------

	for(int j=0; j<tmpImg->matrixSize->height; j++)		delete [] pngBuff[j];
	delete [] pngBuff;

	VOL_DeleteRawImageData(tmpImg);

	return 0;
}