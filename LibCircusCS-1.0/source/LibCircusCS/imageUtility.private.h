////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			imageUtility.private.h : utility for VOL_RAWIMAGEDATA 
//
//			main coder: Y. Nomura
//
//			update history
//
//			2011.03.23	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////


void ConvertRawImageDataToRawVolumeData(VOL_RAWIMAGEDATA* srcData, int srcCh, VOL_RAWVOLUMEDATA* dstData, int dstCh);
void ConvertRawVolumeDataToRawImageData(VOL_RAWVOLUMEDATA* srcData, int srcCh, VOL_RAWIMAGEDATA* dstData, int dstCh);