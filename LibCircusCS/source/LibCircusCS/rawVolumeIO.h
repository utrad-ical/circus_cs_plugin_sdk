////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//		rawVolumeIO.h: I/O function for raw volume file (.raw)  
//
//			main coder: Y. Nomura
//
//			update history
//
//			2012.05.02	start coding (without VOL library)
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef RAW_VOLUME_IO_H

template <typename VARTYPE> VARTYPE* CircusCS_LoadRawVolumeFile(char* fileName, int length);

void* CircusCS_LoadVolumeDataFromMetaHeader(
	char* headerFileName,
	int* voxelUnit,
	CircusCS_INTSIZE3D* matrixSize,
	CircusCS_SIZE3D* voxelSize);

#endif

#define RAW_VOLUME_IO_H
