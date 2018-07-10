#ifndef __cMATH_h__
#define __cMATH_h__

//////////////////////////////////////////////////////////////////////
// MarSTD version 2004 - (c)2004 - Marcel Smit                      //
//                                                                  //
// Marcel_Smit_101@hotmail.com                                      //
// marcel.athlon@hccnet.nl                                          //
//                                                                  //
// This code may not be used in a commercial product without my     //
// permission. If you redistribute it, this message must remain     //
// intact. If you use this code, some acknowledgement would be      //
// appreciated. ;-)                                                 //
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/** @file CMath.h: Common math functions. */
//////////////////////////////////////////////////////////////////////

// NOTE: Default angle type used throughout the librairy is radians.

#include <math.h>
#include <stdlib.h>

//---------------------------------------------------------------------------
// Complement incomplete math headers.
//---------------------------------------------------------------------------

#if !defined(M_PI)
    #if defined(PI)
        #define M_PI PI
    #else
        #define M_PI 3.14159265358979323846		///< Here to complement incomplete math headers.
    #endif
#endif

//---------------------------------------------------------------------------
// Angle conversions.
//---------------------------------------------------------------------------

#define RAD2DEG(_x) ( (_x) / M_PI  * 180.0 )	///< Converts a radian value to degrees.
#define RAD2BIN(_x) ( (_x) / M_PI  * 128.0 )	///< Converts a radian value to binairy (0..256).
#define DEG2RAD(_x) ( (_x) / 180.0 * M_PI  )	///< Converts a degrees value to radians.
#define DEG2BIN(_x) ( (_x) / 180.0 * 128.0 )	///< Converts a degrees value to binairy.
#define BIN2RAD(_x) ( (_x) / 128.0 * M_PI  )	///< Converts a binairy value to radians.
#define BIN2DEG(_x) ( (_x) / 128.0 * 180.0 )	///< Converts a binairy value to degrees.

//---------------------------------------------------------------------------
// Trig.
//---------------------------------------------------------------------------

/**
 * Returns an angle for a given (dx, dy) pair. (dx, dy) is a vector with arbitrary size.
 * cos(angle) = dx.
 * sin(angle) = dy.
 */
static inline float dxdy2rad(float dx, float dy)
{

	if (dy == 0.0)
    {
		if (dx < 0.0)
			return 180.0;
		else
  			return 0.0;			
	}
	
	float dxdy = dx/dy;
	float angle = acos(dxdy);
	
	if (dy < 0.0)
		angle = 2.0*M_PI-angle;
		
	return angle;			
	
}

//---------------------------------------------------------------------------

#endif
