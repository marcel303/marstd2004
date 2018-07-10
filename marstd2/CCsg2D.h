#ifndef __CCsg2D_h__
#define __CCsg2D_h__

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
/** @file CCsg2D.h: CCsg2D definition. */
//////////////////////////////////////////////////////////////////////

#include "CPlane.h"
#include "CPoly.h"

//---------------------------------------------------------------------------

/// Geometry: 2D CSG (Constructive Solid Geometry).
/**
 * The 2D CSG routines are used to add, subtract and intersect 2D geometry, as in polygons.
 * The algoritm makes no assumptions about in which planes the polygons lie. They could even be in different planes, however this would yield incorrect results.
 * One should make sure geometry lies in the same plane - any plane - and everything will work well.
 * Also, one should note CSG operations are only available for convex polygons. Should you wish to use concave object, please deconstruct your geometry first into convex parts and perform the CSG operation on each part seperately.
\code
// Example:

void make_shape(CMesh& mesh)
{

	Poly poly1; // poly1 and poly2 will be added together.
	Poly poly2;
	Poly poly3; // After poly1 and poly2 are added, poly3 will be subtracted.
	
	// my_make_square(CPoly& poly, CVector origin, float size) is a custom function used to create a square with sides length size and centered at origin.
	
	my_make_square(poly1, CVector(-1.0, -1.0, 0.0), 2.0);
	my_make_square(poly2, CVector(+1.0, +1.0, 0.0), 2.0);
	my_make_square(poly3, CVector( 0.0,  0.0, 0.0), 1.0); 	
	
	// Make sure the mesh is clear.
	 
	mesh.clear();
	
	// Addition may yield more than one polygon. So we need a mesh to temporarily store the intermediate result, before we subtract poly3.

	CMesh temp;
	
	CCsg2D::I().addition(&poly1, &poly2, &temp);

	// Subtract poly3 from the intermediate results stored in temp.	This is done by for every polygon in temp, calculating the subtraction of poly3.
	
	for (CPoly* poly = temp.polyHead; poly; poly = poly->next)
		CCsg2D::I().subtraction(poly, &poly3, &mesh);
	
}

\endcode
 */
class CCsg2D
{

	private:
	
	CCsg2D();
	
	public:
	
	~CCsg2D();
	
	public:
	
	/**
     * Use this function to get a reference to the 2d csg singleton object.
     */
	static CCsg2D& I()
	{
		static CCsg2D csg2D;
		return csg2D;
	}
	
	public:

	void addition(CPoly* poly1, CPoly* poly2, CMesh* out); ///< Add two polygons together and store the result in out.
	void subtraction(CPoly* poly1, CPoly* poly2, CMesh* out); ///< Subtract poly2 from poly1, and store the result in out.
	void intersection(CPoly* poly1, CPoly* poly2, CPoly* out); ///< Calculate the intersection (overlapping area) of poly1 and poly2, and store the result in out. Note this function stored it's result in a polygon rather than a mesh, because the intersection of 2 convex polygon can never yield more than 1 (convex) polygon.
	void exclusiveor(CPoly* poly1, CPoly* poly2, CMesh* out); ///< Calculate the inverse of the intersection, and store the result in out.

};

#endif
