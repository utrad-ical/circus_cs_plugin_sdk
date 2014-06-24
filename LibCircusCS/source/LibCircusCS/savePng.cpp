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
//			2012.05.08  rewrite without VOL library
//
//
////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE 

#include "../LibCircusCS.h"

#include "savePng.private.h"

#include "png.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CircusCS_SaveImageAsPng(
	char* fileName,
	unsigned char* img,
	int width,
	int height,
	int type,
	int compressLevel)
{
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

	//----------------------------------------------------------------------------------------------
	// initialize png_struct and png_info
	//----------------------------------------------------------------------------------------------
	png_struct* pngStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!pngStruct)
	{
		fclose(fp);
		fprintf(stderr, "Failed to initialize png_struct.\n");
		return -1;
	}

	png_info* pngInfo = png_create_info_struct(pngStruct);
	if (!pngInfo)
	{
		png_destroy_write_struct(&pngStruct, NULL);
		fclose(fp);
		fprintf(stderr, "Failed to initialize png_info.\n");
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

	int pngColorType = (type == CircusCS_VALUETYPE_SINGLE) ? PNG_COLOR_TYPE_GRAY : PNG_COLOR_TYPE_RGB;

	png_set_IHDR(pngStruct,
		pngInfo,
		width,
		height,
		8,
		pngColorType,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// set pixel data
	//----------------------------------------------------------------------------------------------
	png_bytepp pngBuff;
	pngBuff = new png_byte*[height];

	int chNum = (type == CircusCS_VALUETYPE_SINGLE) ? 1 : 3;

	for(int j=0; j<height; j++)
	{
		pngBuff[j] = new png_byte[width * chNum];

		for(int i=0; i<width; i++)
		{
			if(type == CircusCS_VALUETYPE_RGB)
			{
				int pos = (j * width + i) * 3;	

				pngBuff[j][i * chNum + 0] = img[pos];		// red
				pngBuff[j][i * chNum + 1] = img[pos + 1];	// green
				pngBuff[j][i * chNum + 2] = img[pos + 2];	// blue
			}
			else  // PIXEL_TYPE_GLAYSCALE
			{
				pngBuff[j][i] = img[j * width + i];
			}
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

	for(int j=0; j<height; j++)		delete [] pngBuff[j];
	delete [] pngBuff;

	return 0;
}