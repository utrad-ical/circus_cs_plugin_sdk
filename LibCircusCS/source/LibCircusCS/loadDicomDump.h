
////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			loadDicomDump.h : Load DICOM dump file (.ini format)
//
//			main coder: Y. Nomura
//
//			update history
//
//			2011.03.23	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

#include "../SimpleIni/SimpleIni.h"

#ifndef LOAD_DICOM_DUMP_H

typedef struct DcmDumpData
{
	CSimpleIniA* data;
}CircusCS_DCMDUMPDATA;

typedef struct basicDcmTagValues
{
	char                modality[16];
	char                studyDate[32];
	char                studyTime[32];
	char                seriesDate[32];
	char                seriesTime[32];
	char                sex[8];
	int                 age;
	float               size;
	float               weight;
	CircusCS_INTSIZE3D* matrixSize;
	CircusCS_SIZE3D*    voxelSize_mm;
	int                 pixelUnit;
	float               sliceThickness_mm;
	float*              sliceLocation_mm;
	double		        rescaleSlope;
	double              rescaleIntercept;
}CircusCS_BASICDCMTAGVALUES;


CircusCS_DCMDUMPDATA*	CircusCS_NewDcmDumpData();
void				    CircusCS_DeleteDcmDumpData(CircusCS_DCMDUMPDATA* dumpData);

CircusCS_DCMDUMPDATA*	CircusCS_LoadDcmDumpFile(char* fileName);

CircusCS_BASICDCMTAGVALUES* CircusCS_NewBasicDcmTagValues(char* fileName, int sliceNum=0);
void                        CircusCS_DeleteBasicDcmTagValues(CircusCS_BASICDCMTAGVALUES* values);


std::string CircusCS_GetDcmTagElementOfDumpData(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord);

std::string CircusCS_GetDcmTagElementOfDumpData(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr);

unsigned char*
	CircusCS_GetDcmTagElementOfDumpDataAsUint8Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord);

unsigned char*
	CircusCS_GetDcmTagElementOfDumpDataAsUint8Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr);

std::vector<std::string> CircusCS_GetDcmTagElementOfDumpDataAsStringArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord);

std::vector<std::string> CircusCS_GetDcmTagElementOfDumpDataAsStringArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr);

std::vector<short> CircusCS_GetDcmTagElementOfDumpDataAsSint16Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord);

std::vector<short> CircusCS_GetDcmTagElementOfDumpDataAsSint16Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr);

std::vector<unsigned short> CircusCS_GetDcmTagElementOfDumpDataAsUint16Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord);

std::vector<unsigned short> CircusCS_GetDcmTagElementOfDumpDataAsUint16Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr);

std::vector<int> CircusCS_GetDcmTagElementOfDumpDataAsSint32Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord);

std::vector<int> CircusCS_GetDcmTagElementOfDumpDataAsSint32Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr);

std::vector<unsigned int> CircusCS_GetDcmTagElementOfDumpDataAsUint32Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord);

std::vector<unsigned int> CircusCS_GetDcmTagElementOfDumpDataAsUint32Array(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr);

std::vector<float> CircusCS_GetDcmTagElementOfDumpDataAsFloatArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord);

std::vector<float> CircusCS_GetDcmTagElementOfDumpDataAsFloatArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr);

std::vector<double> CircusCS_GetDcmTagElementOfDumpDataAsDoubleArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord);

std::vector<double> CircusCS_GetDcmTagElementOfDumpDataAsDoubleArray(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr);

std::string CircusCS_GetDcmTagElementOfDumpDataAsString(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	unsigned int   pos = 0);

std::string CircusCS_GetDcmTagElementOfDumpDataAsString(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsSint16(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	short* value,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsSint16(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	short* value,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsUint16(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	unsigned short* value,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsUint16(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	unsigned short* value,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsSint32(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	int* value,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsSint32(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	int* value,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsUint32(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	unsigned int* value,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsUint32(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	unsigned int* value,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsFloat(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	float* value,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsFloat(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	float* value,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsDouble(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	unsigned short groupWord,
	unsigned short elementWord,
	double* value,
	unsigned int pos = 0);

int CircusCS_GetDcmTagElementOfDumpDataAsDouble(
	CircusCS_DCMDUMPDATA* dumpData,
	int sliceNum,
	char* tagStr,
	double* value,
	unsigned int pos = 0);

int	 CircusCS_GetModalityOfDumpData(CircusCS_DCMDUMPDATA* dumpData, char* modality, int sliceNum=0);
int	 CircusCS_GetPatientSexOfDumpData(CircusCS_DCMDUMPDATA* dumpData, char* sex, int sliceNum=0);
int	 CircusCS_GetPatientAgeOfDumpData(CircusCS_DCMDUMPDATA* dumpData, int* age, int sliceNum=0);
int	 CircusCS_GetPatientSizeOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* size, int sliceNum=0);
int	 CircusCS_GetPatientWeightOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* weight, int sliceNum=0);

int	 CircusCS_GetImageNumberOfDumpData(CircusCS_DCMDUMPDATA* dumpData, int* imageNum, int sliceNum=0);
int	 CircusCS_GetAcquisitionNumberOfDumpData(CircusCS_DCMDUMPDATA* dumpData, int* acquisitionNum, int sliceNum=0);

int	 CircusCS_GetMatrixSizeOfDumpData(CircusCS_DCMDUMPDATA* dumpData, CircusCS_INTSIZE2D* matrixSize, int sliceNum=0);
int	 CircusCS_GetFieldOfViewOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* fov, int sliceNum=0);
int	 CircusCS_GetSliceThicknessOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* thickness, int sliceNum=0);
int	 CircusCS_GetDistanceBetweenSlicesOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* dbs, int sliceNum=0);
int	 CircusCS_GetSliceLocationOfDumpData(CircusCS_DCMDUMPDATA* dumpData, float* sliceLoc, int sliceNum=0);
int	 CircusCS_GetPixelSpacingOfDumpData(CircusCS_DCMDUMPDATA* dumpData, CircusCS_SIZE2D* pixelSpacing, int sliceNum=0);
int	 CircusCS_GetPixelUnitOfDicomData(CircusCS_DCMDUMPDATA* dumpData, int* pixelUnit, int sliceNum=0);
int  CircusCS_GetVoxelSizeOfDumpData(CircusCS_DCMDUMPDATA* dumpData, CircusCS_SIZE3D* voxelSize_mm, int sliceNum=0);

int	 CircusCS_GetRescaleInterceptOfDumpData(CircusCS_DCMDUMPDATA* dumpData, double* intercept, int sliceNum=0);
int	 CircusCS_GetRescaleSlopeOfDumpData(CircusCS_DCMDUMPDATA* dumpData, double* slope, int sliceNum=0);

int	 CircusCS_GetRadionuclideTotalDoseOfDumpData(CircusCS_DCMDUMPDATA* dumpData, double* totalDose, int sliceNum=0);

std::vector<float> CircusCS_GetImageOrientationOfDumpData(CircusCS_DCMDUMPDATA* dumpData, int sliceNum=0);
std::vector<float> CircusCS_GetImagePositionOfDumpData(CircusCS_DCMDUMPDATA* dumpData, int sliceNum=0);

#endif

#define LOAD_DICOM_DUMP_H
