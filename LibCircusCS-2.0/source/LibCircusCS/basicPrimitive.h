////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			basicPrimitive.h: for basic primitives such as "value" and "vector"
//
//			main coder: Y. Nomura
//
//			update history
//
//			2012.05.07	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef BASIC_PRIMITIVE_H

// Pixel unit
#define CircusCS_VALUEUNIT_UINT8		 0	// unsigned char
#define CircusCS_VALUEUNIT_SINT8		 1	// (signed) char
#define CircusCS_VALUEUNIT_UINT16		 2	// unsigned short
#define CircusCS_VALUEUNIT_SINT16		 3	// (signed) short
#define CircusCS_VALUEUNIT_UINT32		 4	// unsigned int
#define CircusCS_VALUEUNIT_SINT32		 5	// (signed) int
#define CircusCS_VALUEUNIT_FLOAT32		 6	// float
#define CircusCS_VALUEUNIT_FLOAT64		 7	// double
#define CircusCS_VALUEUNIT_UNKNOWN		-1	// unknown

// Value type
#define CircusCS_VALUETYPE_SINGLE		 0
#define CircusCS_VALUETYPE_RGB			 1

// Section
#define AXIAL_SECTION					 0
#define CORONAL_SECTION					 1
#define SAGITTAL_SECTION				 2


typedef struct Size2D		{ float	width, height; }			CircusCS_SIZE2D;
typedef struct IntSize2D	{ int	width, height;	}			CircusCS_INTSIZE2D;
typedef struct Size3D		{ float	width, height, depth; }		CircusCS_SIZE3D;
typedef struct IntSize3D	{ int	width, height, depth; }		CircusCS_INTSIZE3D;


CircusCS_SIZE2D*		CircusCS_NewSize2D(float width, float height);
void					CircusCS_DeleteSize2D(CircusCS_SIZE2D* vector);

CircusCS_INTSIZE2D*		CircusCS_NewIntSize2D(int width, int height);
void					CircusCS_DeleteIntSize2D(CircusCS_INTSIZE2D* size);

CircusCS_SIZE3D*		CircusCS_NewSize3D(float width, float height, float depth);
void					CircusCS_DeleteSize3D(CircusCS_SIZE3D* vector);

CircusCS_INTSIZE3D*		CircusCS_NewIntSize3D(int width, int height, int depth);
void					CircusCS_DeleteIntSize3D(CircusCS_INTSIZE3D* size);

#endif

#define BASIC_PRIMITIVE_H

