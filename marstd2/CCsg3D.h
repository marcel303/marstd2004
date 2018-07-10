#ifndef __CCsg3D_h__
#define __CCsg3D_h__

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
/** @file CCsg3D.h: CCsg3D definition. */
//////////////////////////////////////////////////////////////////////

#include "CBsp.h"
#include "CPlane.h"
#include "CPoly.h"

//---------------------------------------------------------------------------

/// Geometry: 3D CSG (Constructive Solid Geometry).
/**
 * FIXME: Add description here.
\code
// Example:

FIXME: Add example code here.
\endcode
 */
class CCsg3D
{

	private:
	
	CCsg3D();
	
	public:
	
	~CCsg3D();
	
	public:
	
	/**
     * Use this function to get a reference to the 3d csg singleton object.
     */
	static CCsg3D& I()
	{
		static CCsg3D csg3D;
		return csg3D;
	}
	
	public:

	void addition(CMesh* mesh1, CMesh* mesh2, CMesh* out);
	void subtraction(CMesh* mesh1, CMesh* mesh2, CMesh* out);
	void intersection(CMesh* mesh1, CMesh* mesh2, CMesh* out);
	void exclusiveOr(CMesh* mesh1, CMesh* mesh2, CMesh* out);

	private:

	void clip(CMesh* mesh, CMesh* clipMesh, CMesh* out, CMesh* in, int polyClipPriority);
	void clip(CMesh* mesh, CBsp* bsp, CMesh* out, CMesh* in, int polyClipPriority);

};

#endif
