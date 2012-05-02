////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//		volumeOneFileIO.h: I/O function for Volume-One file (.vol)  
//
//			main coder: Y. Nomura
//
//			update history
//
//			2011.03.23	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef VOLUME_ONE_FILE_IO_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

VOL_RAWVOLUMEDATA*	CircusCS_LoadVolumeOneFile(char* fileName);
int					CircusCS_SaveVolumeOneFile(char* fileName, VOL_RAWVOLUMEDATA* volume);

VOL_VECTOR3D*	CircusCS_LoadCursorMemory(char* fileName, int* numMemory);
int				CircusCS_SaveCursorMemory(char* fileName, VOL_MATRIX* pos);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

#define VOLUME_ONE_FILE_IO_H
