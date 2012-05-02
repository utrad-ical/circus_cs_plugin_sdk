////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//		volumeOneFileIO.cpp: I/O function for Volume-One file (.vol)  
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

#include "volumeOneFileIO.private.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOL_RAWVOLUMEDATA* 
CircusCS_LoadVolumeOneFile(char* fileName)
{
	FILE	*fp;

	fp = fopen(fileName, "rb");
	if(fp == NULL)
	{
		fprintf(stderr, "Fail to open the file (%s)\n", fileName);
		return NULL;
	}

	VOL_RAWVOLUMEDATA*	volume;
	VOL_INTSIZE3D		matrix;
	int					ch, size;

	//------------------------------------------------------------------------------------------------------------------
	// Read Headder
	//------------------------------------------------------------------------------------------------------------------
	{
		char buf[MAX_STRING_LENGTH];
		char *tp;
	
		// Headder check
		fgets(buf, MAX_STRING_LENGTH, fp);
		if(strcmp(buf, "# VOLUME-ONE raw format file with header\n") != 0)
		{
			fprintf(stderr, "Error: This file is not Volume-One raw format file with header!!\n");
			return NULL;
		}

		// Read matrix size
		fgets(buf, MAX_STRING_LENGTH, fp);
		sscanf(buf, "%d %d %d %d", &matrix.width, &matrix.height, &matrix.depth, &ch);
		
		int *vUnit = new int[ch];
		int *vType = new int[ch];

		//------------------------------------------------------------------------------------------
		// Read line for voxel unit
		//------------------------------------------------------------------------------------------
		fgets(buf, MAX_STRING_LENGTH, fp);
		tp = strtok(buf, " ");
		vUnit[0] = atoi(tp);

		for(int i=1; i<ch; i++)		vUnit[i] = atoi(strtok(NULL, " "));
		//------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------------------
		// Read line for voxel type
		//------------------------------------------------------------------------------------------
		fgets(buf, MAX_STRING_LENGTH, fp);
		tp = strtok(buf, " ");
		vType[0] = atoi(tp);

		for(int i=1; i<ch; i++)		vType[i] = atoi(strtok(NULL, " "));
		//------------------------------------------------------------------------------------------

		volume = VOL_NewSingleChannelRawVolumeData(&matrix, vUnit[0], vType[0]);
		for(int i=1; i<ch; i++)  VOL_AddNewChannelInRawVolumeData(volume, vUnit[i], vType[i]);
		size = VOL_GetNumberOfVoxels(volume);

		fgets(buf, MAX_STRING_LENGTH, fp);

		delete [] vUnit, vType;
	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Read volume data
	//------------------------------------------------------------------------------------------------------------------
	for(int i=0; i<ch; i++)
	{
		switch(volume->voxelUnit[i])
		{
			case 0: // VOL_VALUEUNIT_UINT8
			fread((char *)volume->data[i], sizeof(unsigned char), size, fp);
			break;

			case 1: // VOL_VALUEUNIT_SINT8
			fread((char *)volume->data[i], sizeof(char), size, fp);
			break;

			case 2: // VOL_VALUEUNIT_UINT16
			fread((char *)volume->data[i], sizeof(unsigned short), size, fp);
			break;

			case 3: // VOL_VALUEUNIT_SINT16
			fread((char *)volume->data[i], sizeof(short), size, fp);
			break;

			case 4: // VOL_VALUEUNIT_UINT32
			fread((char *)volume->data[i], sizeof(unsigned int), size, fp);
			break;

			case 5: // VOL_VALUEUNIT_SINT32
			fread((char *)volume->data[i], sizeof(int), size, fp);
			break;

			case 6: // VOL_VALUEUNIT_FLOAT32
			fread((char *)volume->data[i], sizeof(float), size, fp);
			break;

			case 7: // VOL_VALUEUNIT_FLOAT64
			fread((char *)volume->data[i], sizeof(double), size, fp);
			break;
		}
	}
	//------------------------------------------------------------------------------------------------------------------

	fclose(fp);
	
	return volume;
}


int
CircusCS_SaveVolumeOneFile(char* fileName, VOL_RAWVOLUMEDATA* volume)
{
	FILE	*fp;

	fp = fopen(fileName, "wb");
	if(fp == NULL)
	{
		fprintf(stderr, "Fail to open the file (%s)\n", fileName);
		return -1;
	}

	VOL_INTSIZE3D*  matrix = VOL_GetIntSize3DFromIntSize4D(volume->matrixSize);
	int				size = VOL_GetNumberOfVoxels(volume);
	int				ch = volume->matrixSize->channel;

	//------------------------------------------------------------------------------------------------------------------
	// Write Header
	//------------------------------------------------------------------------------------------------------------------
	{
		fprintf(fp, "# VOLUME-ONE raw format file with header\n");
		fprintf(fp, "%d %d %d %d\n", matrix->width, matrix->height, matrix->depth, ch);

		for(int i=0; i<ch; i++)		fprintf(fp, "%d ", volume->voxelUnit[i]);
		fprintf(fp, "\n");

		for(int i=0; i<ch; i++)		fprintf(fp, "%d ", volume->voxelType[i]);
		fprintf(fp, "\n");

		int bytes = 0;

		for(int i=0; i<ch; i++)
		{
			switch(volume->voxelUnit[i])
			{
				case 0:  bytes += sizeof(unsigned char);   break;  // VOL_VALUEUNIT_UINT8
				case 1:  bytes += sizeof(char);            break;  // VOL_VALUEUNIT_SINT8
				case 2:  bytes += sizeof(unsigned short);  break;  // VOL_VALUEUNIT_UINT16
				case 3:  bytes += sizeof(short);           break;  // VOL_VALUEUNIT_SINT16
				case 4:  bytes += sizeof(unsigned int);    break;  // VOL_VALUEUNIT_UINT32
				case 5:  bytes += sizeof(int);             break;  // VOL_VALUEUNIT_SINT32
				case 6:  bytes += sizeof(float);           break;  // VOL_VALUEUNIT_FLOAT32
				case 7:  bytes += sizeof(double);          break;  // VOL_VALUEUNIT_FLOAT64
			}
		}

		fprintf(fp, "%d uncompressed\n", size*bytes);

	}
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// Write volume data
	//------------------------------------------------------------------------------------------------------------------
	for(int i=0; i<ch; i++)
	{
		switch(volume->voxelUnit[i])
		{
			case 0: // VOL_VALUEUNIT_UINT8
			fwrite((char*)volume->data[i], sizeof(unsigned char), size, fp);
			break;

			case 1: // VOL_VALUEUNIT_SINT8
			fwrite((char*)volume->data[i], sizeof(char), size, fp);
			break;

			case 2: // VOL_VALUEUNIT_UINT16
			fwrite((char*)volume->data[i], sizeof(unsigned short), size, fp);
			break;

			case 3: // VOL_VALUEUNIT_SINT16
			fwrite((char*)volume->data[i], sizeof(short), size, fp);
			break;

			case 4: // VOL_VALUEUNIT_UINT32
			fwrite((char*)volume->data[i], sizeof(unsigned int), size, fp);
			break;

			case 5: // VOL_VALUEUNIT_SINT32
			fwrite((char*)volume->data[i], sizeof(int), size, fp);
			break;

			case 6: // VOL_VALUEUNIT_FLOAT32
			fwrite((char*)volume->data[i], sizeof(float), size, fp);
			break;

			case 7: // VOL_VALUEUNIT_FLOAT64
			fwrite((char*)volume->data[i], sizeof(double), size, fp);
			break;
		}
	}
	//------------------------------------------------------------------------------------------------------------------

	fclose(fp);

	VOL_DeleteIntSize3D(matrix);

	return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOL_VECTOR3D*
CircusCS_LoadCursorMemory(char* fileName, int* numMemory)
{
	FILE  *fp;
	char   buffer[MAX_STRING_LENGTH];
	char  *tp;
	int	   i, j;

	fp = fopen(fileName, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "Fail to open the file (%s)\n", fileName);
		return NULL;
	}

	fgets(buffer, MAX_STRING_LENGTH, fp);
	*numMemory = atoi(buffer);
	if( *numMemory > MAX_CURSOR_MEMORY ) *numMemory = MAX_CURSOR_MEMORY;

	VOL_VECTOR3D* Cursor = new VOL_VECTOR3D[*numMemory];

	for(i=0;i<(*numMemory);i++)
	{
		for(j=0; j<3; j++)	fgets(buffer, MAX_STRING_LENGTH, fp);

		// Read Cursor Position
		tp = strtok(buffer, ","); 
		Cursor[i].x = (float)atof(tp);

		tp = strtok(NULL, ","); 
		Cursor[i].y = (float)atof(tp);

		tp = strtok(NULL, ","); 
		Cursor[i].z = (float)atof(tp);
		
		for(j=0; j<4; j++)	fgets(buffer, MAX_STRING_LENGTH, fp);
	}

	fclose(fp);

	return Cursor;
}


int
CircusCS_SaveCursorMemory(char* fileName, VOL_MATRIX* pos)
{
	FILE  *fp;

	fp = fopen(fileName, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "Fail to open the file (%s)\n", fileName);
		return -1;
	}

	unsigned short numMemory = (unsigned short)pos->m;

	if(0 < numMemory)
	{
		if( numMemory > MAX_CURSOR_MEMORY ) numMemory = MAX_CURSOR_MEMORY;
	}
	else	return -1;

	fprintf(fp, "%d\n", numMemory);

	int numTP = 1, numFP = 1;

	for(unsigned short j=0; j<numMemory; j++)
	{
		fprintf(fp, "============================================\n");

		fprintf(fp, "%d\n", j+1);

		fprintf(fp, "%d,%d,%d\n", (int)(pos->data[j][0]+0.5f), (int)(pos->data[j][1]+0.5f), (int)(pos->data[j][2]+0.5f));

		for(unsigned short i=0; i<4; i++)	fprintf(fp, "---\n");
	}

	fclose(fp);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
