#ifndef __Cpoly_h__
#define __Cpoly_h__

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
/** @file CPoly.h: CPoly and CMesh implementation. */
//////////////////////////////////////////////////////////////////////

#include "CEdge.h"
#include "CMatrix.h"
#include "CSphere.h"

//---------------------------------------------------------------------------

/// Geometry: Polygon type.
/**
 * The polygon class defines a polygon using a DLL of edges in CW order.
 * Polygons should be convex and defined CW to use all of the features.
 */
class CPoly
{

    public:
 
	CPoly();
	~CPoly();
	
    public:
 
	CEdge* edgeHead;	///< First edge in DLL.
	CEdge* edgeTail;	///< Last edge in DLL.
 	int edgeCount;		///< Number of edges.
	CPlane plane; 		///< Plane. Uses left hand rule.
	CSphere sphere;
	void* data;         ///< Custom data field. Use it to store a pointer to some custom data structure. Eg to define material properties, texture, ...
	
    public:
 
	/**
	 * Links an edge to the tail.
	 * @see add_tail()
  	 */ 
 	CEdge* add(CEdge* edge);
 	/**
 	 * Links an edge to the head.
 	 * @see add_tail()
 	 */
	CEdge* addHead(CEdge* edge);
	/**
	 * Links an edge to the tail.
	 * @see add_head()
	 */
 	CEdge* addTail(CEdge* edge);
 	/**
 	 * Unlinks an edge from the DLL. Note that this does not free any memory.
 	 * @see remove()
 	 */
 	CEdge* unlink(CEdge* edge);
 	/**
 	 * Removes an edge. This unlinks the edge and then frees it's memory.
 	 * @unlink()
 	 */
 	void remove(CEdge* edge);
 	/**
 	 * Clear the DLL of edges. This will free all memory.
 	 */
 	void clear();

	void copy(CPoly* poly);
	CPoly* copy();
 	
  	/**
  	 * Initializes the polygon. This will calculate the plane and edge normals.
  	 * This should be done when the polygon creation has finished. If the number of
  	 * edges < 3 this function will bail.
  	 */
 	void init();
	/**
	 * Aligns the polygon's vertices to the polygon's plane. This makes sure rounding errors are minimalised
	 * and vertices lie as exactly as possible on the same plane.
	 */
	void align();
 	/**
 	 * Calculates a bounding sphere. The polygon is entirely contained within the
   	 * bounding sphere.
   	 * @return The bounding sphere.
 	 */
 	CSphere calculateSphere();
	/**
     * Returns true if the polygon is in front of the plane, considering epsilon.
     */
	int onFront(CPlane* plane);
	/**
     * Returns true if the polygon is behind of the plane, considering epsilon.
     */
	int onBack(CPlane* plane);
	/**
     * Clip the polygon by plane, returning the remaining polygon (all that is in front of the plane).
     * FIXME: changing this to clip everything BEHIND the plane will probably affect brush code. Will have to fix it. :)
     */
	CPoly* clip(CPlane* plane);
	/**
     * Same as clip(CPlane* plane), however this removes the part that is behind the plane.
     */
	CPoly* clipFront(CPlane* plane);
	/**
     * Clip the polygon by plane, returning both the parts that are in front and behind the plane.
     */
	void clip(CPlane* plane, CPoly* front, CPoly* back);
	/**
     * Clip the polygon by another polygon that is coplanar, possibly overlapping.
     * out = this - poly.
     * in = this - out.
\code
For instance:

++++                ++++
++---- => outside = ++    inside =   ++  poly =   ----
++----              ++               ++           ----
  ----                                            ----
\endcode
     */
	void clip(CPoly* poly, CPoly* inside, class CMesh* outside);
	
	public:
	
	void reverseWinding();
	void triangulate(std::vector<CPoly*>* polyArray);
 	
    public:
 
	CPoly* prev;	///< Reference to previous polygon in DLL.
 	CPoly* next;	///< Reference to next polygon in DLL.
  
    public:
    
 	/**
 	 * Unlinks this polygon from a DLL.
 	 */
	CPoly* unlink();

};

//////////////////////////////////////////////////////////////////////

/// Geometry: Mesh type.
/**
 * A mesh is just a collection of polygons. Use it to store many polygons belonging to a particular group,
 * so you can render them using a different matrix eg, or to paint them with the same material.
 */
class CMesh
{

    public:
    
	CMesh();
	~CMesh();
	
    public:
 
   	CPoly* polyHead;	///< First polygon in DLL.
   	CPoly* polyTail;	///< Last polygon in DLL.
   	int polyCount;	    ///< Number of polygons in DLL.
   	void* data;         ///< Custom data field.
	
    public:
    
 	/**
 	 * Links polygon to tail.
 	 * @see add_tail()
 	 */
 	CPoly* add(CPoly* poly);
 	/**
 	 * Links polygon to head.
 	 * @see add_tail()
 	 */
	CPoly* addHead(CPoly* poly);
 	/**
 	 * Links polygon to tail.
 	 * @see add_head()
 	 */	
	CPoly* addTail(CPoly* poly);
 	/**
     * Unlinks poygon from DLL.
     */
	CPoly* unlink(CPoly* poly);
	/**
	 * Unlinks polygon from DLL and frees memory.
	 */
	void remove(CPoly* poly);
	/**
	 * Clear polygon DLL.
	 */
	void clear();
	void copy(CMesh* mesh);
	CMesh* copy();
	void triangulate(CMesh* mesh);
    /** Transforms all vertices in mesh.
     * @param matrix Matrix that will be used to transform vertices.
     */
	void transform(CMatrix* matrix);
	/**
	 * Moves contents of polygons DLL to another mesh.
	 */
	void move(CMesh* mesh);
	/**
	 * Assign a custom data reference to all polygon.
	 * @param data Pointer to custom data structure.
	 */
	void paint(void* data);

};

//---------------------------------------------------------------------------

#endif
