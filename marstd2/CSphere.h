#ifndef __cSPHERE_h__
#define __cSPHERE_h__

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
/** @file CSphere.h: CSphere implementation. */
//////////////////////////////////////////////////////////////////////

#include "CVector.h"
#include "CList.h"

//---------------------------------------------------------------------------

/// Geometry: Sphere type.
/**
 * Sphere type. Generic position, radius.
 */
class CSphere
{

    public:
    
	CSphere()
    {
		radius = 0.0;
		prev = 0;
		next = 0;
	} 
	~CSphere()
    {
		unlink();
	}
	
    public:
 
    CVector position;
    float radius;		///< Radius of sphere. Any point for which (point - position).size() == radius lies on sphere.
  
    public:
    
	/**
	 * Copies sphere.
	 */
	void operator=(CSphere sphere)
    {
		position = sphere.position;
		radius = sphere.radius;
	} 
	
    public:
 
    CSphere* prev;
 	CSphere* next;
 	
	CSphere* unlink()
    {
		DLLIST_UNLINK_SELF();
		return this;
	}    	

};

//---------------------------------------------------------------------------

#endif
