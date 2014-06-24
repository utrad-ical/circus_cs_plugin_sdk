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

/*
*  Load raw volume data
*/
template <typename VARTYPE> VARTYPE* CircusCS_LoadRawVolumeFile(char* fileName, int length)
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


/*
*  Load volume data from meta header
*/
void* CircusCS_LoadVolumeDataFromMetaHeader(
	char* headerFileName,
	int* voxelUnit,
	CircusCS_INTSIZE3D* matrixSize,
	CircusCS_SIZE3D* voxelSize)
{
	char  rawFileName[512];
	char  drive[512], dir[512], path[512];
	char  buffer[512], key[512], value[512];
	char* tp;

	// Extract path from headerFileName
	_splitpath(headerFileName, drive, dir, NULL, NULL);
	strcat(drive, dir);
	strcpy(path, drive);

	*voxelUnit = CircusCS_VALUEUNIT_SINT8; 

	FILE* fp = fopen(headerFileName, "r");

	if(fp == NULL)
	{
		fprintf(stderr, "Failed to open the file (%s)\n", headerFileName);
		return NULL;
	}

	while(fgets(buffer, 1024, fp) != NULL)
	{
		tp = strtok(buffer, "=");
		strcpy(key, tp);
		trimSpace(key);

		tp = strtok(NULL, "=");
		strcpy(value, tp);
		trimSpace(value);

		if(strcmp(key, "ElementType") == 0)
		{
			fprintf(stderr, "ElementType: %s\n", value);

			if(strcmp(value, "MET_UCHAR") == 0)        *voxelUnit = CircusCS_VALUEUNIT_UINT8;
			else if(strcmp(value, "MET_SHORT") == 0)   *voxelUnit = CircusCS_VALUEUNIT_SINT16;
			else if(strcmp(value, "MET_USHORT") == 0)  *voxelUnit = CircusCS_VALUEUNIT_UINT16;
			else if(strcmp(value, "MET_INT") == 0)     *voxelUnit = CircusCS_VALUEUNIT_SINT32;
			else if(strcmp(value, "MET_UINT") == 0)    *voxelUnit = CircusCS_VALUEUNIT_UINT32;
			else if(strcmp(value, "MET_FLOAT") == 0)   *voxelUnit = CircusCS_VALUEUNIT_FLOAT32;
			else if(strcmp(value, "MET_DOUBLE") == 0)  *voxelUnit = CircusCS_VALUEUNIT_FLOAT64;
		}
		else if(strcmp(key, "DimSize") == 0)
		{

			tp = strtok(value, " ");
			matrixSize->width  = atoi(tp);

			tp = strtok(NULL, " ");
			matrixSize->height = atoi(tp);

			tp = strtok(NULL, " ");
			matrixSize->depth  = atoi(tp);

			fprintf(stderr, "Matrix size: %d x %d x %d\n", matrixSize->width,
				matrixSize->height,
				matrixSize->depth);
		}
		else if(strcmp(key, "ElementSpacing") == 0)
		{
			tp = strtok(value, " ");
			voxelSize->width = (float)atof(tp);

			tp = strtok(NULL, " ");
			voxelSize->height = (float)atof(tp);

			tp = strtok(NULL, " ");
			voxelSize->depth  = (float)atof(tp);

			fprintf(stderr, "Voxel size [mm]: %f x %f x %f\n", voxelSize->width,
				voxelSize->height,
				voxelSize->depth);
		}
		else if(strcmp(key, "ElementDataFile") == 0)
		{
			fprintf(stderr, "ElementDataFile: %s\n", value);
			sprintf(rawFileName, "%s\\%s", path, value);
		} 
	}
	fclose(fp);

	void* ret    = NULL;
	int   length = matrixSize->width * matrixSize->height * matrixSize->depth;

	switch(*voxelUnit)
	{
	case CircusCS_VALUEUNIT_UINT8:   ret = (void*)CircusCS_LoadRawVolumeFile<unsigned char>(rawFileName, length);  break;
	case CircusCS_VALUEUNIT_SINT8:   ret = (void*)CircusCS_LoadRawVolumeFile<char>(rawFileName, length);           break;
	case CircusCS_VALUEUNIT_UINT16:  ret = (void*)CircusCS_LoadRawVolumeFile<unsigned short>(rawFileName, length); break;
	case CircusCS_VALUEUNIT_SINT16:  ret = (void*)CircusCS_LoadRawVolumeFile<short>(rawFileName, length);          break;
	case CircusCS_VALUEUNIT_UINT32:  ret = (void*)CircusCS_LoadRawVolumeFile<unsigned int>(rawFileName, length);   break;
	case CircusCS_VALUEUNIT_SINT32:  ret = (void*)CircusCS_LoadRawVolumeFile<int>(rawFileName, length);            break;
	case CircusCS_VALUEUNIT_FLOAT32: ret = (void*)CircusCS_LoadRawVolumeFile<float>(rawFileName, length);          break;
	case CircusCS_VALUEUNIT_FLOAT64: ret = (void*)CircusCS_LoadRawVolumeFile<double>(rawFileName, length);         break;
	}

	if(ret == NULL)
	{
		fprintf(stderr, "Failed to load raw volume data: %s\n", rawFileName);
	}

	return ret;
}


/*
* Trim  whitespace (including tab) and newline characters from target string
* @param[in] str Target string
* @return The number of trimed characters
*/
int trimSpace(char* str)
{
	int i;
	int cnt = 0;

	// check null pointer
	if(str == NULL)  return -1;

	// get length of the string
	i = (int)strlen(str);

	// trim from end
	while(--i >= 0 
		&& (str[i] == ' ' || str[i] == '\v' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n'))
	{
		cnt++;
	}
	str[i+1] = '\0';

	// trim from start
	i = 0;
	while(str[i] != '\0' && (str[i] == ' ' || str[i] == '\v' || str[i] == '\t'))
	{
		i++;
	}
	strcpy(str, &str[i]);

	return i + cnt;
}
