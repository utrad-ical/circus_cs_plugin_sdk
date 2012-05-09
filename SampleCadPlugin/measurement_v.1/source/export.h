

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int exportImageFilesFromVolumeData(char* jobRootPath,
								   short* orgVolume,
								   unsigned char* resultVolume,
								   CircusCS_INTSIZE3D* matrixSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */