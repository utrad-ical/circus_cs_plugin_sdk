////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//		volumeOneFileIO.private.h: I/O function for Volume-One file (.vol)  
//
//			main coder: Y. Nomura
//
//			update history
//
//			2011.03.23	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////


#define MAX_CURSOR_MEMORY		256





VOL_VECTOR3D*	LoadCursorMemory(char* fname, int* numMemory);
int				SaveCursorMemory(char* fname, VOL_MATRIX* pos);