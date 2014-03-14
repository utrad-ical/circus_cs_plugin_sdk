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
CircusCS_LoadRawVolumeFile(char* fileName, int length)
{
	VARTYPE* ret = (VARTYPE*)calloc(length, sizeof(VARTYPE));

	if(ret == NULL)
	{
		fprintf(stderr, "Failed to allocate output\n");
		return NULL;
	}

	FILE* fp = fopen(fileName, "rb");
	
	if(fp == NULL)
	{
		fprintf(stderr, "Failed to open the file (%s)\n", fileName);
	}
	else
	{
		fread((char *)ret, sizeof(VARTYPE), length, fp);
		fclose(fp);
	}

	return ret;
}

template unsigned char*  CircusCS_LoadRawVolumeFile<unsigned char>(char* fileName, int length);
template char*           CircusCS_LoadRawVolumeFile<char>(char* fileName, int length);
template unsigned short* CircusCS_LoadRawVolumeFile<unsigned short>(char* fileName, int length);
template short*          CircusCS_LoadRawVolumeFile<short>(char* fileName, int length);
template unsigned int*   CircusCS_LoadRawVolumeFile<unsigned int>(char* fileName, int length);
template int*            CircusCS_LoadRawVolumeFile<int>(char* fileName, int length);
