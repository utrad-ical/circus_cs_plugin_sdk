////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			loadDicomDump.private.h : Load DICOM dump file (.ini format)
//
//			main coder: Y. Nomura
//
//			update history
//
//			2011.03.23	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////

unsigned char* base64Decode(const char* src);

std::string getTagElementData(CircusCS_DCMDUMPDATA* dumpData, int sliceNum, char* tagStr);

