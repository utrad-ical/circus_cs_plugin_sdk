////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			loadDicomDump.cpp : Load DICOM dump file (.ini format)
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

#include "loadDicomDump.h"
#include "loadDicomDump.private.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CircusCS_DCMDUMPDATA* CircusCS_NewDcmDumpData()
{
	CircusCS_DCMDUMPDATA* ret;

	ret = (CircusCS_DCMDUMPDATA*)malloc(sizeof(CircusCS_DCMDUMPDATA));

	ret->data = new CSimpleIniA[1];

	return ret;
}

void CircusCS_DeleteDcmDumpData(CircusCS_DCMDUMPDATA* dumpData)
{
	dumpData->data->~CSimpleIniTempl();
	free(dumpData);
}


CircusCS_DCMDUMPDATA* CircusCS_LoadDcmDumpFile(char* fileName)
{
	CircusCS_DCMDUMPDATA* ret = CircusCS_NewDcmDumpData();

	if(ret->data->LoadFile(fileName) != SI_OK)
	{
		CircusCS_DeleteDcmDumpData(ret);
		return NULL;
	}

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> splitString(std::string str, std::string delim)
{
	std::vector<std::string> ret;

	for(unsigned int i=0, n; i <= str.length(); i=n+1 )
	{
		n = (unsigned int)str.find_first_of(delim, i);
		if( n == std::string::npos ) n = (unsigned int)str.length();
		std::string tmp = str.substr( i, n-i );
		ret.push_back(tmp);
	}
	return ret;
}


int checkBase64Encoding(CircusCS_DCMDUMPDATA* dumpData, int sliceNum, char* tagStr)
{
	char sectionStr[128];
	sprintf(sectionStr, "%d", sliceNum);

	std::string ret = dumpData->data->GetValue(sectionStr, tagStr, "");

	if(ret.compare("") == 0)
	{
		ret = dumpData->data->GetValue("common", tagStr, "");
	}

	// Decode Base64 encoding
	if(ret.compare(0,8,"{Base64}") == 0)
	{
		return 1;
	}

	return 0;
}

template<typename VARTYPE> std::vector<VARTYPE> getDcmTagElementOfDumpDataAsArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr)
{
	VARTYPE buf;
	std::vector<VARTYPE> ret;

	std::vector<std::string> dstVec = splitString(getTagElementData(dumpData, sliceNum, tagStr), "\\");

	int encodeFlg = checkBase64Encoding(dumpData, sliceNum, tagStr);

	for(unsigned int i=0; i<dstVec.size(); i++)
	{
		if(encodeFlg)
		{
			memcpy(&buf, dstVec[i].c_str(), sizeof(VARTYPE));
			fprintf(stderr, "%s ",  dstVec[i].c_str());
		}
		else
		{
			if(typeid(VARTYPE) == typeid(float) || typeid(VARTYPE) == typeid(double))
			{
				buf = (VARTYPE)atof(dstVec[i].c_str());
			}
			else
			{
				buf = (VARTYPE)atoi(dstVec[i].c_str());
			}
		}
		ret.push_back(buf);
	}

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CircusCS_GetDcmTagElementOfDumpData(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return getTagElementData(dumpData, sliceNum, tagStr);
}

std::string CircusCS_GetDcmTagElementOfDumpData(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr)
{
	return getTagElementData(dumpData, sliceNum, tagStr);
}

unsigned char* CircusCS_GetDcmTagElementOfDumpDataAsUint8Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return  CircusCS_GetDcmTagElementOfDumpDataAsUint8Array(dumpData, sliceNum, tagStr);
}

unsigned char* CircusCS_GetDcmTagElementOfDumpDataAsUint8Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr)
{
	return  (unsigned char*)(getTagElementData(dumpData, sliceNum, tagStr).c_str());
}

std::vector<std::string> CircusCS_GetDcmTagElementOfDumpDataAsStringArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return  CircusCS_GetDcmTagElementOfDumpDataAsStringArray(dumpData, sliceNum, tagStr);
}

std::vector<std::string> CircusCS_GetDcmTagElementOfDumpDataAsStringArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr)
{
	return splitString(getTagElementData(dumpData, sliceNum, tagStr), "\\");
}

std::vector<short> CircusCS_GetDcmTagElementOfDumpDataAsSint16Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return  CircusCS_GetDcmTagElementOfDumpDataAsSint16Array(dumpData, sliceNum, tagStr);
}

std::vector<short> CircusCS_GetDcmTagElementOfDumpDataAsSint16Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr)
{
	std::vector<short> ret;
	ret = getDcmTagElementOfDumpDataAsArray<short>(dumpData, sliceNum, tagStr);
	return ret;
}

std::vector<unsigned short> CircusCS_GetDcmTagElementOfDumpDataAsUint16Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return CircusCS_GetDcmTagElementOfDumpDataAsUint16Array(dumpData, sliceNum, tagStr);
}

std::vector<unsigned short> CircusCS_GetDcmTagElementOfDumpDataAsUint16Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr)
{
	std::vector<unsigned short> ret;
	ret = getDcmTagElementOfDumpDataAsArray<unsigned short>(dumpData, sliceNum, tagStr);
	return ret;
}

std::vector<int> CircusCS_GetDcmTagElementOfDumpDataAsSint32Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return  CircusCS_GetDcmTagElementOfDumpDataAsSint32Array(dumpData, sliceNum, tagStr);
}

std::vector<int> CircusCS_GetDcmTagElementOfDumpDataAsSint32Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr)
{
	std::vector<int> ret;
	ret = getDcmTagElementOfDumpDataAsArray<int>(dumpData, sliceNum, tagStr);
	return ret;
}

std::vector<unsigned int> CircusCS_GetDcmTagElementOfDumpDataAsUint32Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return CircusCS_GetDcmTagElementOfDumpDataAsUint32Array(dumpData, sliceNum, tagStr);
}

std::vector<unsigned int> CircusCS_GetDcmTagElementOfDumpDataAsUint32Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr)
{
	std::vector<unsigned int> ret;
	ret = getDcmTagElementOfDumpDataAsArray<unsigned int>(dumpData, sliceNum, tagStr);
	return ret;
}

std::vector<float> CircusCS_GetDcmTagElementOfDumpDataAsFloatArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return CircusCS_GetDcmTagElementOfDumpDataAsFloatArray(dumpData, sliceNum, tagStr);
}

std::vector<float> CircusCS_GetDcmTagElementOfDumpDataAsFloatArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr)
{
	std::vector<float> ret;
	ret = getDcmTagElementOfDumpDataAsArray<float>(dumpData, sliceNum, tagStr);
	return ret;
}

std::vector<double> CircusCS_GetDcmTagElementOfDumpDataAsDoubleArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return CircusCS_GetDcmTagElementOfDumpDataAsDoubleArray(dumpData, sliceNum, tagStr);
}

std::vector<double> CircusCS_GetDcmTagElementOfDumpDataAsDoubleArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr)
{
	std::vector<double> ret;
	ret = getDcmTagElementOfDumpDataAsArray<double>(dumpData, sliceNum, tagStr);
	return ret;
}

std::string CircusCS_GetDcmTagElementOfDumpDataAsString(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	unsigned int pos)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return CircusCS_GetDcmTagElementOfDumpDataAsString(dumpData, sliceNum, tagStr, pos);
}

std::string CircusCS_GetDcmTagElementOfDumpDataAsString(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	unsigned int pos)
{
	std::string ret;
	std::vector<std::string> buf;

	buf = CircusCS_GetDcmTagElementOfDumpDataAsStringArray(dumpData, sliceNum, tagStr);

	if(buf.size() == 0 || buf.size() <= pos)	 return NULL;

	return buf[pos];

}

int CircusCS_GetDcmTagElementOfDumpDataAsSint16(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	short* value,
	unsigned int pos)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return CircusCS_GetDcmTagElementOfDumpDataAsSint16(dumpData, sliceNum, tagStr, value, pos);

}

int CircusCS_GetDcmTagElementOfDumpDataAsSint16(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	short* value,
	unsigned int pos)
{
	std::vector<short> buf;

	buf = CircusCS_GetDcmTagElementOfDumpDataAsSint16Array(dumpData, sliceNum, tagStr);

	if(buf.size() == 0 || buf.size() <= pos)	 return -1;

	*value = buf[pos];

	return 0;
}


int CircusCS_GetDcmTagElementOfDumpDataAsUint16(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	unsigned short* value,
	unsigned int pos)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return CircusCS_GetDcmTagElementOfDumpDataAsUint16(dumpData, sliceNum, tagStr, value, pos);
}

int	CircusCS_GetDcmTagElementOfDumpDataAsUint16(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	unsigned short* value,
	unsigned int pos)
{
	std::vector<unsigned short> buf;

	buf = CircusCS_GetDcmTagElementOfDumpDataAsUint16Array(dumpData, sliceNum, tagStr);

	if(buf.size() == 0 || buf.size() <= pos)	 return -1;

	*value = buf[pos];

	return 0;
}

int CircusCS_GetDcmTagElementOfDumpDataAsSint32(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	int* value,
	unsigned int pos)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return CircusCS_GetDcmTagElementOfDumpDataAsSint32(dumpData, sliceNum, tagStr, value, pos);
}

int CircusCS_GetDcmTagElementOfDumpDataAsSint32(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	int* value,
	unsigned int pos)
{
	std::vector<int> buf;

	buf = CircusCS_GetDcmTagElementOfDumpDataAsSint32Array(dumpData, sliceNum, tagStr);

	if(buf.size() == 0 || buf.size() <= pos)	 return -1;

	*value = buf[pos];

	return 0;
}


int CircusCS_GetDcmTagElementOfDumpDataAsUint32(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	unsigned int* value,
	unsigned int pos)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return CircusCS_GetDcmTagElementOfDumpDataAsUint32(dumpData, sliceNum, tagStr, value, pos);
}

int CircusCS_GetDcmTagElementOfDumpDataAsUint32(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	unsigned int* value,
	unsigned int pos)
{
	std::vector<unsigned int> buf;

	buf = CircusCS_GetDcmTagElementOfDumpDataAsUint32Array(dumpData, sliceNum, tagStr);

	if(buf.size() == 0 || buf.size() <= pos)	 return -1;

	*value = buf[pos];

	return 0;
}


int CircusCS_GetDcmTagElementOfDumpDataAsFloat(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	float* value,
	unsigned int pos)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return CircusCS_GetDcmTagElementOfDumpDataAsFloat(dumpData, sliceNum, tagStr, value, pos);
}

int CircusCS_GetDcmTagElementOfDumpDataAsFloat(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	float* value,
	unsigned int pos)
{
	std::vector<float> buf;

	buf = CircusCS_GetDcmTagElementOfDumpDataAsFloatArray(dumpData, sliceNum, tagStr);

	if(buf.size() == 0 || buf.size() <= pos)	 return -1;

	*value = buf[pos];

	return 0;
}


int CircusCS_GetDcmTagElementOfDumpDataAsDouble(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	double* value,
	unsigned int pos)
{
	char tagStr[128];
	sprintf(tagStr, "%04x,%04x", groupWord, elementWord);

	return CircusCS_GetDcmTagElementOfDumpDataAsDouble(dumpData, sliceNum, tagStr, value, pos);
}

int CircusCS_GetDcmTagElementOfDumpDataAsDouble(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	double* value,
	unsigned int pos)
{
	std::vector<double> buf;

	buf = CircusCS_GetDcmTagElementOfDumpDataAsDoubleArray(dumpData, sliceNum, tagStr);

	if(buf.size() == 0 || buf.size() <= pos)	 return -1;

	*value = buf[pos];

	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CircusCS_BASICDCMTAGVALUES* CircusCS_NewBasicDcmTagValues(char* fileName, int sliceNum)
{
	std::string                 buf;
	CircusCS_BASICDCMTAGVALUES* ret;
	CircusCS_DCMDUMPDATA*       dumpData;

	if( (ret=(CircusCS_BASICDCMTAGVALUES*)malloc(sizeof(CircusCS_BASICDCMTAGVALUES))) == NULL )
	{
		return NULL;
	}

	if( (ret->matrixSize = CircusCS_NewIntSize3D(0, 0, 0)) == NULL)
	{
		free(ret);
		return NULL;
	}

	if( (ret->voxelSize_mm = CircusCS_NewSize3D(0.0f, 0.0f, 0.0f)) == NULL)
	{
		CircusCS_DeleteIntSize3D(ret->matrixSize);
		free(ret);
		return NULL;
	}

	if( (dumpData = CircusCS_LoadDcmDumpFile(fileName)) == NULL)
	{
		CircusCS_DeleteBasicDcmTagValues(ret);
		return NULL;
	}

	// Modality
	CircusCS_GetModalityOfDumpData(dumpData, ret->modality, sliceNum);

	// Study/Series date and time
	buf = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_StudyDate);
	strcpy(ret->studyDate, buf.c_str());

	buf = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_StudyTime);
	strcpy(ret->studyTime, buf.c_str());

	buf = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_SeriesDate);
	strcpy(ret->seriesDate, buf.c_str());

	buf = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_SeriesTime);
	strcpy(ret->seriesTime, buf.c_str());

	// Sex, age, size, weight
	CircusCS_GetPatientSexOfDumpData(dumpData, ret->sex, sliceNum);
	CircusCS_GetPatientAgeOfDumpData(dumpData, &ret->age, sliceNum);
	CircusCS_GetPatientSizeOfDumpData(dumpData, &ret->size, sliceNum);
	CircusCS_GetPatientWeightOfDumpData(dumpData, &ret->weight, sliceNum);

	// Matrix size
	CircusCS_INTSIZE2D matrixSize;

	CircusCS_GetMatrixSizeOfDumpData(dumpData, &matrixSize, sliceNum);

	ret->matrixSize->width  = matrixSize.width;
	ret->matrixSize->height = matrixSize.height;

	// Get the number of slice
	{
		CSimpleIniA::TNamesDepend sections;
		CSimpleIniA::TNamesDepend::const_iterator pp;

		dumpData->data->GetAllSections(sections);
		ret->matrixSize->depth = (int)sections.size();

		for(pp = sections.begin(); pp != sections.end(); pp++)
		{
			if(strcmp(pp->pItem, "common") == 0)
			{
				ret->matrixSize->depth--;
				break;
			}
		}
	}

	// Voxel size (mm)
	CircusCS_GetVoxelSizeOfDumpData(dumpData, ret->voxelSize_mm, sliceNum);

	// Pixel unit
	CircusCS_GetPixelUnitOfDicomData(dumpData, &ret->pixelUnit, sliceNum);

	// Slice thickness
	CircusCS_GetSliceThicknessOfDumpData(dumpData, &ret->sliceThickness_mm, sliceNum);

	// Rescale slope
	CircusCS_GetRescaleSlopeOfDumpData(dumpData, &ret->rescaleSlope, sliceNum);

	// Rescale intercept
	CircusCS_GetRescaleInterceptOfDumpData(dumpData, &ret->rescaleIntercept, sliceNum);

	// slice location
	if( (ret->sliceLocation_mm = (float*)malloc(ret->matrixSize->depth * sizeof(float))) == NULL)
	{
		CircusCS_DeleteSize3D(ret->voxelSize_mm);
		CircusCS_DeleteIntSize3D(ret->matrixSize);
		free(ret);
		return NULL;
	}

	for(int i=0; i<ret->matrixSize->depth; i++)
	{
		CircusCS_GetSliceLocationOfDumpData(dumpData, &ret->sliceLocation_mm[i]);
	}

	CircusCS_DeleteDcmDumpData(dumpData);

	return ret;
}


void CircusCS_DeleteBasicDcmTagValues(CircusCS_BASICDCMTAGVALUES* values)
{
	CircusCS_DeleteSize3D(values->voxelSize_mm);
	CircusCS_DeleteIntSize3D(values->matrixSize);
	free(values->sliceLocation_mm);
	free(values);
}


int CircusCS_GetModalityOfDumpData(CircusCS_DCMDUMPDATA* dumpData, char* modality, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_Modality);

	if(buffer.compare("") == 0)	return -1;

	strcpy(modality, buffer.c_str());
	return 0;
}


int CircusCS_GetPatientSexOfDumpData(CircusCS_DCMDUMPDATA* dumpData, char* sex, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_PatientSex);

	if(buffer.compare("") == 0)	return -1;

	strcpy(sex, buffer.c_str());
	return 0;
}

int CircusCS_GetPatientAgeOfDumpData(CircusCS_DCMDUMPDATA* dumpData, int* age, int sliceNum)
{
	*age = 0;
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_PatientAge);

	if(buffer.compare("") == 0)	 return -1;

	if(buffer.find("Y", 0) != std::string::npos)
	{
		buffer.erase(3,1);
		*age = atoi(buffer.c_str());
	}

	return 0;
}


int CircusCS_GetPatientSizeOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* size, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_PatientSize);

	if(buffer.compare("") == 0)	return -1;

	*size = (float)atof(buffer.c_str());

	return 0;
}

int CircusCS_GetPatientWeightOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* weight, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_PatientWeight);

	if(buffer.compare("") == 0)	return -1;

	*weight = (float)atof(buffer.c_str());

	return 0;
}


int CircusCS_GetImageNumberOfDumpData(CircusCS_DCMDUMPDATA* dumpData, int* imageNum, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_InstanceNumber);

	if(buffer.compare("") == 0)	return -1;

	*imageNum = atoi(buffer.c_str());

	return 0;
}


int CircusCS_GetAcquisitionNumberOfDumpData(CircusCS_DCMDUMPDATA* dumpData, int* acquisitionNum, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_AcquisitionNumber);

	if(buffer.compare("") == 0)	return -1;

	*acquisitionNum = atoi(buffer.c_str());

	return 0;
}


int CircusCS_GetMatrixSizeOfDumpData(CircusCS_DCMDUMPDATA* dumpData, CircusCS_INTSIZE2D* matrixSize, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_Columns);

	if(buffer.compare("") == 0)	return -1;

	matrixSize->width = atoi(buffer.c_str());	

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_Rows);

	if(buffer.compare("") == 0)	return -1;

	matrixSize->height = atoi(buffer.c_str());	

	return 0;
}

int CircusCS_GetFieldOfViewOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* fov, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_ReconstructionDiameter);

	if(buffer.compare("") == 0)	return -1;

	*fov = (float)atof(buffer.c_str());

	return 0;
}


int CircusCS_GetSliceThicknessOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* thickness, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_SliceThickness);

	if(buffer.compare("") == 0)	return -1;

	*thickness = (float)atof(buffer.c_str());

	return 0;
}


int CircusCS_GetDistanceBetweenSlicesOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* dbs, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_SpacingBetweenSlices);

	if(buffer.compare("") == 0)	return -1;

	*dbs = (float)atof(buffer.c_str());

	return 0;
}


int CircusCS_GetSliceLocationOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* sliceLoc, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_SliceLocation);

	if(buffer.compare("") == 0)	return -1;

	*sliceLoc = (float)atof(buffer.c_str());

	return 0;
}


int CircusCS_GetPixelSpacingOfDumpData(CircusCS_DCMDUMPDATA* dumpData, CircusCS_SIZE2D* pixelSpacing, int sliceNum)
{
	char *tp;
	char buffer[MAX_STRING_LENGTH];

	strcpy(buffer, CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_PixelSpacing).c_str());

	if(strcmp(buffer, "") == 0)  return -1;

	tp =  strtok(buffer, "\\");
	pixelSpacing->width = (float)atof(tp);

	tp =  strtok(NULL, "\\");
	pixelSpacing->height = (float)atof(tp);

	return 0;
}

int CircusCS_GetPixelUnitOfDicomData(CircusCS_DCMDUMPDATA* dumpData, int* pixelUnit, int sliceNum)
{
	char buffer[MAX_STRING_LENGTH];

	// Get BitsAllocated (0x0028, 0x0100)
	strcpy(buffer, CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_BitsAllocated).c_str());

	if(strcmp(buffer, "") == 0)  return -1;
	int bits = atoi(buffer);

	// Get PixelRepresentation (0x0028, 0x0103)
	strcpy(buffer, CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_PixelRepresentation).c_str());
	int sign = (strcmp(buffer, "") == 0) ? 0 : atoi(buffer);  // 1 : Signed, 0 : Unsigned

	if(bits <= 8)
	{
		if(sign)	*pixelUnit = CircusCS_VALUEUNIT_SINT8;
		else		*pixelUnit = CircusCS_VALUEUNIT_UINT8;
		return 0;
	}
	else if(bits <=16)
	{
		if(sign)	*pixelUnit = CircusCS_VALUEUNIT_SINT16;
		else		*pixelUnit = CircusCS_VALUEUNIT_UINT16;
		return 0;
	}
	else
	{
		*pixelUnit = CircusCS_VALUEUNIT_UNKNOWN;
		return -1;
	}
}


int CircusCS_GetVoxelSizeOfDumpData(CircusCS_DCMDUMPDATA* dumpData, CircusCS_SIZE3D* voxelSize_mm, int sliceNum)
{
	CircusCS_SIZE2D pixelSpacing_mm;
	char       modality[64];

	if(CircusCS_GetPixelSpacingOfDumpData(dumpData, &pixelSpacing_mm, sliceNum)==-1)  return -1;

	voxelSize_mm->width  = pixelSpacing_mm.width;
	voxelSize_mm->height = pixelSpacing_mm.height;

	if(CircusCS_GetModalityOfDumpData(dumpData, modality, sliceNum)==-1) return -1;

	if(strncmp(modality, "MR", 2)==0)
	{
		// Get distance between slices 
		if( CircusCS_GetDistanceBetweenSlicesOfDumpData(dumpData, &voxelSize_mm->depth, sliceNum) == -1
			|| voxelSize_mm->depth <= 0.0f || voxelSize_mm->depth > 10.0f )
		{
			fprintf(stderr,"Warning: Distance between Slices is not found or invalid (%f) in DICOM (IMAGE_POSITION used)\n",
				voxelSize_mm->depth);

			std::vector<float> imagePosition0 = CircusCS_GetImagePositionOfDumpData(dumpData, sliceNum);
			std::vector<float> imagePosition1 = CircusCS_GetImagePositionOfDumpData(dumpData, sliceNum+1);

			if(imagePosition0.empty() || imagePosition1.empty())
			{
				fprintf(stderr, "IMAGE_POSITION is not found in DICOM dump data\n");
				return -1;
			}

			imagePosition0[0] -= imagePosition1[0];
			imagePosition0[1] -= imagePosition1[1];
			imagePosition0[2] -= imagePosition1[2];

			voxelSize_mm->depth = (float)sqrt(imagePosition0[0] * imagePosition0[0]
			+ imagePosition0[1] * imagePosition0[1]
			+ imagePosition0[2] * imagePosition0[2]);
		}
	}
	else
	{
		float sliceLocation0, sliceLocation1;	

		if(CircusCS_GetSliceLocationOfDumpData(dumpData, &sliceLocation0, sliceNum)==-1)   return -1;
		if(CircusCS_GetSliceLocationOfDumpData(dumpData, &sliceLocation1, sliceNum+1)==-1) return -1;

		voxelSize_mm->depth = abs(sliceLocation1 - sliceLocation0);
	}

	return 0;
}


int CircusCS_GetRescaleInterceptOfDumpData(CircusCS_DCMDUMPDATA* dumpData, double* intercept, int sliceNum)
{
	std::string buffer;
	*intercept = 0.0;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_RescaleIntercept);

	if(buffer.compare("") == 0) return -1;

	*intercept = atof(buffer.c_str());

	return 0;
}

int CircusCS_GetRescaleSlopeOfDumpData(CircusCS_DCMDUMPDATA* dumpData, double* slope, int sliceNum)
{
	std::string buffer;
	*slope = 1.0;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_RescaleSlope);

	if(buffer.compare("") == 0)	return -1;

	*slope = atof(buffer.c_str());

	return 0;
}


int CircusCS_GetRadionuclideTotalDoseOfDumpData(CircusCS_DCMDUMPDATA* dumpData, double* totalDose, int sliceNum)
{
	std::string buffer;

	buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_RadionuclideTotalDose);

	if(buffer.compare("") == 0)
	{
		char tagStr[128];
		sprintf(tagStr, "%s->%s", TAGSTR_RadiopharmaceuticalInformationSequence,
			TAGSTR_RadionuclideTotalDose);

		buffer = CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, tagStr);

		if(buffer.compare("") == 0)  return -1;
	}

	*totalDose = atof(buffer.c_str());
	return 0;
}


std::vector<float> CircusCS_GetImageOrientationOfDumpData(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum)
{
	char *tp;
	char buffer[MAX_STRING_LENGTH];
	std::vector<float> ret;

	strcpy(buffer, CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_ImageOrientationPatient).c_str());

	if(strcmp(buffer, "") == 0)  return ret;

	tp =  strtok(buffer, "\\");
	ret.push_back((float)atof(tp));

	for (int i=1; i<=5; i++)
	{
		tp = strtok(NULL, "\\");
		ret.push_back((float)atof(tp));
	}

	return ret;
}

std::vector<float> CircusCS_GetImagePositionOfDumpData(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum)
{
	char *tp;
	char buffer[MAX_STRING_LENGTH];
	std::vector<float> ret;

	strcpy(buffer, CircusCS_GetDcmTagElementOfDumpData(dumpData, sliceNum, TAGSTR_ImagePositionPatient).c_str());

	if(strcmp(buffer, "") == 0)  return ret;

	tp =  strtok(buffer, "\\");
	ret.push_back((float)atof(tp));

	tp =  strtok(NULL, "\\");
	ret.push_back((float)atof(tp));

	tp =  strtok(NULL, "\\");
	ret.push_back((float)atof(tp));	

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string getTagElementData(CircusCS_DCMDUMPDATA* dumpData, int sliceNum, char* tagStr)
{
	char sectionStr[128];
	sprintf(sectionStr, "%d", sliceNum);

	std::string ret = dumpData->data->GetValue(sectionStr, tagStr, "");

	if(ret.compare("") == 0)
	{
		ret = dumpData->data->GetValue("common", tagStr, "");
	}

	// Decode Base64 encoding
	if(ret.compare(0,8,"{Base64}") == 0)
	{
		ret = (char*)base64Decode(ret.substr(8).c_str());
	}

	//fprintf(stderr, "---%s %s\n", tagStr, ret.c_str());

	return ret;
}


// Base64 translation table as described in RFC 2045 (MIME)
static const char BASE64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned char* base64Decode(const char* src)
{
	unsigned char* ret = NULL;
	unsigned char  ind1, ind2, ind3, ind4;

	int size = sizeof(src[0])*(int)strlen(src);    

	ret = (unsigned char*)calloc(sizeof(unsigned char), size + 3);

	for(int i=0, j=0; i< size ; i += 4 , j += 3)
	{       
		ind1 = *(src + i + 0) == '=' ? 0 : (unsigned char)(strchr(BASE64, *(src + i + 0)) - BASE64); // same as codetovalue
		ind2 = *(src + i + 1) == '=' ? 0 : (unsigned char)(strchr(BASE64, *(src + i + 1)) - BASE64);
		ind3 = *(src + i + 2) == '=' ? 0 : (unsigned char)(strchr(BASE64, *(src + i + 2)) - BASE64);
		ind4 = *(src + i + 3) == '=' ? 0 : (unsigned char)(strchr(BASE64, *(src + i + 3)) - BASE64);

		// decode
		*(ret + j + 0) = (unsigned char)( (ind1 & 0x3f) << 2 | (ind2 & 0x30) >> 4 );
		*(ret + j + 1) = (unsigned char)( (ind2 & 0x0f) << 4 | (ind3 & 0x3c) >> 2 );
		*(ret + j + 2) = (unsigned char)( (ind3 & 0x03) << 6 | (ind4 & 0x3f) >> 0 );
	}

	return ret;
}