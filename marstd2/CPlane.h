#ifndef __cPLANE_h__
#define __cPLANE_h__

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
/** @file CPlane.h: CPlane implementation. */
//////////////////////////////////////////////////////////////////////

#include <vector>
#include "CVector.h"
#include "CList.h"

//---------------------------------------------------------------------------

/**
 * The relation between a plane and some other geometric shape.
 */
enum CPlaneClassification
	{
		pOn,		///< Shape in exactly on the plane.
		pFront,		///< Shape is completely in front of plane.
		pBack,		///< Shape is completely behind plane.
		pSpan		///< Shape is spanning plane.
	};

/// Geometry: Plane type.
/**
 * Plane type. Generic normal(x, y, z) + distance.
 */
class CPlane
{

	public:
 
	CPlane()
	{
		prev = 0;
		next = 0;
	} 
	~CPlane()
	{
		unlink();
	}
	
	public:
 
	CVector normal;		///< Normal vector.
 	float distance;		///< 'Distance' or d value of equation.
  
	public:
 
	/**
 	 * Normalises the normal, and adjusts distance appropiately.
     */
 	void normalize()
	{
 		float size = normal.size();
 		if (size == 0.0)
 			return;
 		normal /= size;
 		distance /= size;
 	}
 	/**
 	 * Sets up a plane from three given vectors. These vectors define a triangle in CW order.
 	 * @param vector1 First point of triangle.
 	 * @param vector2 Second point of triangle.
 	 * @param vector3 Third point of triangle.    	 
 	 */
	void setup(CVector vector1, CVector vector2, CVector vector3)
	{
		CVector delta1 = vector2 - vector1;
		CVector delta2 = vector3 - vector2;
		
		normal = delta1 % delta2;
		
		distance = normal * vector1;
	}
     
	public:

	/**
	 * Copies plane.
	 */
	void operator=(CPlane plane)
	{
		normal = plane.normal;
		distance = plane.distance;
	} 
	/**
	 * Returns distance of vector to plane.
	 * @return The distance: normal * vector - distance.
	 */
	float operator*(CVector vector)
	{
		return normal * vector - distance;
	}
	/**
	 * Negates (mirrors) plane.
	 */
	CPlane operator-()
	{
		CPlane tmp;
		tmp.normal = -normal;
		tmp.distance = -distance;
		return tmp;
	}
	
	public:
 
	CPlane* prev;
 	CPlane* next;  	
 	
	public:
 
	CPlane* unlink()
	{
		DLLIST_UNLINK_SELF();
		return this;
	}

};

//---------------------------------------------------------------------------

/**
 * CPlane vector. (STL)
 */
typedef std::vector<CPlane> CPlaneArray;

/**
 * CPlane vector iterator. (STL)
 */
typedef CPlaneArray::iterator CPlaneArrayItr;
 
//---------------------------------------------------------------------------

#endif
