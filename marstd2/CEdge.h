#ifndef __cEDGE_h__
#define __cEDGE_h__

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
/** @file CEdge.h: CEdge implementation. */
//////////////////////////////////////////////////////////////////////

// FIXME: Add edge2 member which will point to edge with next vertex.

#include "CList.h"
#include "CPlane.h"
#include "CVertex.h"

//---------------------------------------------------------------------------

/// Geometry: Edge type.
/**
 * Edges are used by polygons to define their contours using vectors.
 * An edge also has 3 planes. These are used to optimize collision detetcion, etc.
 */
class CEdge : public CVertex
{

	public:
 
	CEdge() : CVertex()
	{
		edge2 = 0;
		prev = 0;
		next = 0;
	} 
	~CEdge()
	{

	}
	
	public:

	CPlane plane;		///< Plane perpendicular to polygon normal and edge.
	CPlane edge_plane;	///< Plane parallel to edge. edge_plane * edge2->p == 1.0.
	CPlane poly_plane;	///< Average plane of all planes of the polygons that share this point.
	CEdge* edge2;		///< Edge that will give next point.
	
	public:

	/**
 	 * Copies vector.
   	 */ 
	void operator=(CEdge& edge)
	{
		CVertex::operator=(edge);
		plane = edge.plane;
		edge_plane = edge.edge_plane;
	}
	
	public:
 
	CEdge* prev;		///< Previous edge in DLL.
	CEdge* next;		///< Next edge in DLL.
	
	public:
 
	/**
   	 * Unlinks this edge from DLL.
         */ 
	CEdge* unlink()
	{
		DLLIST_UNLINK_SELF();
	}  	

};

//---------------------------------------------------------------------------

#endif
