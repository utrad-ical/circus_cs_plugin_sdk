////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//		rawVolumeIO.cpp: I/O function for raw volume file (.raw)  
//
//			main coder: Y. Nomura
//
//			update history
//
//			2012.05.02	start coding (without VOL library)
//
//
////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE 

#include "../LibCircusCS.h"

#include "rawVolumeIO.private.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename VARTYPE> VARTYPE*  
CircusCS_LoadRawVolumeFile(char* fileName, int size)
{
	VARTYPE* ret = (VARTYPE*)calloc(size, sizeof(VARTYPE));

	if(ret == NULL)
	{
		fprintf(stderr, "Fail to allocate output\n");
		return NULL;
	}

	FILE* fp = fopen(fileName, "rb");
	
	if(fp == NULL)
	{
		fprintf(stderr, "Fail to open the file (%s)\n", fileName);
	}
	else
	{
		fread((char *)ret, sizeof(VARTYPE), size, fp);
		fclose(fp);
	}

	return ret;
}


unsigned char*
CircusCS_LoadRawVolumeFileAsUint8(char* fileName, int size)
{
	unsigned char* ret = CircusCS_LoadRawVolumeFile<unsigned char>(fileName, size);
	return ret;
}

char*
CircusCS_LoadRawVolumeFileAsSint8(char* fileName, int size)
{
	char* ret = CircusCS_LoadRawVolumeFile<char>(fileName, size);
	return ret;
}

unsigned short*
CircusCS_LoadRawVolumeFileAsUint16(char* fileName, int size)
{
	unsigned short* ret = CircusCS_LoadRawVolumeFile<unsigned short>(fileName, size);
	return ret;
}

short*
CircusCS_LoadRawVolumeFileAsSint16(char* fileName, int size)
{
	short* ret = CircusCS_LoadRawVolumeFile<short>(fileName, size);
	return ret;
}

unsigned int*
CircusCS_LoadRawVolumeFileAsUint32(char* fileName, int size)
{
	unsigned int* ret = CircusCS_LoadRawVolumeFile<unsigned int>(fileName, size);
	return ret;
}

int*
CircusCS_LoadRawVolumeFileAsSint32(char* fileName, int size)
{
	int* ret = CircusCS_LoadRawVolumeFile<int>(fileName, size);
	return ret;
}

