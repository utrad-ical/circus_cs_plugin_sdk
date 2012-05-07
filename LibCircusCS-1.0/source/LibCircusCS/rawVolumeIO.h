////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//		rawVolumeIO.h: I/O function for raw volume file (.vol)  
//
//			main coder: Y. Nomura
//
//			update history
//
//			2012.05.02	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef RAW_VOLUME_IO_H

template <typename T> T* CircusCS_LoadRawVolumeFile(char* fileName, int size);

template unsigned char*  CircusCS_LoadRawVolumeFile(char* fileName, int size);
template char*           CircusCS_LoadRawVolumeFile(char* fileName, int size);
template unsigned short* CircusCS_LoadRawVolumeFile(char* fileName, int size);
template short*          CircusCS_LoadRawVolumeFile(char* fileName, int size);
template unsigned int*   CircusCS_LoadRawVolumeFile(char* fileName, int size);
template int*            CircusCS_LoadRawVolumeFile(char* fileName, int size);


#endif

#define RAW_VOLUME_IO_H
