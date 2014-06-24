////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			basicPrimitive.cpp: for basic primitives such as "value" and "vector"
//
//			main coder: Y. Nomura
//
//			update history
//
//			2012.05.07	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "../LibCircusCS.h"
#include "basicPrimitive.private.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CircusCS_SIZE2D* CircusCS_NewSize2D(float width, float height)
{
	CircusCS_SIZE2D* ret = (CircusCS_SIZE2D*)malloc(sizeof(CircusCS_SIZE2D));

	if( ret != NULL ) { ret->width = width; ret->height = height;}
	return ret;
}

void CircusCS_DeleteSize2D(CircusCS_SIZE2D* size) { free(size); }


CircusCS_INTSIZE2D* CircusCS_NewIntSize2D(int width, int height)
{
	CircusCS_INTSIZE2D* ret = (CircusCS_INTSIZE2D*)malloc(sizeof(CircusCS_INTSIZE2D));

	if( ret != NULL ){ ret->width = width; ret->height = height;}
	return ret;
}

void CircusCS_DeleteIntSize2D(CircusCS_INTSIZE2D* size) { free(size); }


CircusCS_SIZE3D* CircusCS_NewSize3D(float width, float height, float depth)
{
	CircusCS_SIZE3D* ret = (CircusCS_SIZE3D*)malloc(sizeof(CircusCS_SIZE3D));

	if( ret != NULL ) { ret->width = width; ret->height = height; ret->depth = depth; }
	return ret;
}

void CircusCS_DeleteSize3D(CircusCS_SIZE3D* size) { free(size); }


CircusCS_INTSIZE3D* CircusCS_NewIntSize3D(int width, int height, int depth)
{
	CircusCS_INTSIZE3D* ret = (CircusCS_INTSIZE3D*)malloc(sizeof(CircusCS_INTSIZE3D));

	if( ret != NULL ) { ret->width = width; ret->height = height; ret->depth = depth; }
	return ret;
}

void CircusCS_DeleteIntSize3D(CircusCS_INTSIZE3D* size) { free(size); }

