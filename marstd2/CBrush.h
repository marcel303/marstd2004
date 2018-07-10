#ifndef __Cbrush_h__
#define __Cbrush_h__

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
/** @file CBrush.h: CBrush implementation. */
//////////////////////////////////////////////////////////////////////

#include "CList.h"
#include "CPlane.h"
#include "CPoly.h"

/// Geometry: Brush class.
/**
 * The brush class is used to define a convex volume enclosed by planes. The brush is defined by planes and can
 * be converted to a mesh. Note the planes' normals must point outwards for a valid volume to be enclosed. Brushes can be added, subtracted and or-ed together using CSG.
\code
// Example:

void make_random_shape(CMesh& mesh, float radius) {

	// Probably not the most optimized way to do this. ;)
	
	CBrush brush;
	CVector origin;	// Origin = (0.0, 0.0, 0.0).
	
	for (int i = 0; i < 3; i++)
	{
	
		CPlane* plane = brush.add(new CPlane);
		
		// Make a random normal.
		
		for (int j = 0; j < 3; j++)
			plane->normal[j] = (rand() & 1023) / 1023.0 - 0.5;
			
   		plane.normal.normalize();
   		
   		// Offset plane from origin.
   		
		plane->distance	= radius;		
		
		// Make sure every plane's normal points away from origin.
		
		if (plane * origin > 0.0)
			plane = - plane;
	
	}
	
	// Convert brush to mesh, and move polygons to output mesh.
	
	CMesh* tmp = brush.mesh();
	
	tmp->move(mesh);
	
	delete tmp;

}
\endcode 
 */
class CBrush
{

    public:

	CBrush();
	~CBrush();

    public:

	CPlane* planeHead;     ///< First plane in DLL.
	CPlane* planeTail;     ///< Last plane in DLL.
	int planeCount;        ///< Number of planes in DLL.
    void* data;            ///< Custom data field.

    public:

	CPlane* add(CPlane* plane);
	CPlane* addHead(CPlane* plane);
	CPlane* addTail(CPlane* plane);
	void remove(CPlane* plane);
	CPlane* unlink(CPlane* plane);
	void clear();

    public:

	bool convex(); ///< Returns true if this is a convex brush.
	/**
	 * Creates an "initial" polygon. An initial polygon is a polygon that has a very large size and lies on the specified plane. This "initial" polygon will be used further to clip against the brush's other planes to create one of the brush's side faces.
	 * @param plane The plane we will create the initial polygon for. Assumes the plane is normalized.
	 */
	CPoly* initial(CPlane* plane);
	/**
     * Creates a polygon (brush side) for the given plane.
     * @param plane The plane we will create the brush side for.
     */
	CPoly* poly(CPlane* plane);
	/**
     * Convert the brush to a mesh.
     */
	CMesh* mesh();
	/**
     * Return the corner points of the brush converted to wireframe. This will NOT remove duplicate points. 
     */
	CEdge* points();

    public:

 	CBrush* copy();              ///< Returns a copy of the brush.
	CBrush* clip(CPlane* plane); ///< Clips the brush, returning the resulting brush.
	
    public:
 
	CBrush* prev;
 	CBrush* next;
  
    public:
 
	CBrush* unlink();

};

#endif
