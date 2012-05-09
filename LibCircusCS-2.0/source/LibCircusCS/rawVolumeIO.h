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

unsigned char*  CircusCS_LoadRawVolumeFileAsUint8(char* fileName, int size);
char*           CircusCS_LoadRawVolumeFileAsSint8(char* fileName, int size);
unsigned short* CircusCS_LoadRawVolumeFileAsUint16(char* fileName, int size);
short*          CircusCS_LoadRawVolumeFileAsSint16(char* fileName, int size);
unsigned int*   CircusCS_LoadRawVolumeFileAsUint32(char* fileName, int size);
int*            CircusCS_LoadRawVolumeFileAsSint32(char* fileName, int size);

#endif

#define RAW_VOLUME_IO_H
