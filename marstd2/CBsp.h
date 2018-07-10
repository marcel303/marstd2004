#ifndef __cBSP_h__
#define __cBSP_h__

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
/** @file CBsp.h: CBsp implementation. */
//////////////////////////////////////////////////////////////////////

#include "CDebug.h"
#include "CVector.h"
#include "CPlane.h"
#include "CPoly.h"
#include "CSphere.h"

//#define BSP_ALTERNATE_CANCEL  ///< If uncommented, an alternate 'bail out method' will be used. This bail out method will not create a complete BSP, in that it will gaurantee full front-to-back sorting. Polygons that are not coplanar might end up in the same leaf node.
#define BSP_USE_SPHERES       ///< If uncommented, bounding spheres will be used for faster polygon classification against planes. (Eg. on front, on back, spanning).
#define BSP_DEFAULT_EPS	0.001 ///< Epsilon value to keep floating point happy and minimize splits. This value should generally be as small as possible, without breaking the proper function of the BSP compiler.

/**
 * The CBspSplitInfo structure is used to store the results of "a try" to split a BSP node with a specific plane. The BSP compiler uses these statistics to determine the "optimal" splitting plane before actually splitting itself. Which plane will be optimal depends on the test() function of the BSP class.
 */
typedef struct
{

	CPlane plane; ///< Splitting plane.
	int planeFlipped; ///< Set to 1 if the plane derived from a polygon was flipped when performing the test.

	int fc; ///< Number of polygons in front of plane, after split.
	int bc; ///< Number of polygons behind plane, after split.
	int dc; ///< Difference of front and back polys.
	int sc; ///< Number of polygons spanning plane.

} CBspSplitInfo;

/**
 * The CBspPolyData structure is used to encapsulate a polygon's origional data member and provide extra fields needed by the BSP compiler. After the compiler will be done with the polygons, the origional data members will be restored.
 */
typedef struct
{

	void* data;			///< Old data pointer of poly.
	CSphere sphere;		///< Bubble around the origional polygon.
	
} CBspPolyData;

/// Geometry: BSP class.
/**
 * BSP (Binary Space Partition) class. This class will take any mesh and create a BSP from it.
 * Common uses are optimising collision detection, maximising overdraw on accelerated platforms, or z-sorting on software platforms.<BR>
\code
// Example:

void create_bsp(CBsp& bsp)
{

	// Add a donut shape.
	
	CGeomBuilder::I().donut(bsp, 10, 10, 2.0, 1.0);
	
	// Add a square.
	
	CPoly* poly = bsp.add(new CPoly);
	
	for (int i = 0; i < 4; i++)
	{
		CEdge* egde = poly->add(new CEdge);
		edge->p[0] = sin(i / 4.0 * M_PI);
		edge->p[1] = cos(i / 4.0 * M_PI);
	}
	
	poly->init();	// Very important. This will set up the plane, which is neccessary for BSP to work.
	
	// Partition BSP.
	
	bsp.split();
	
}
\endcode
 * @author Marcel Smit
 */
class CBsp : public CMesh
{

	public:

	CBsp();
	~CBsp();

	public:

 	CPlane plane;		///< Splitting plane.
	int planeFlipped;
 	CBsp* child[2];		///< Front & back children.
 	CBsp* parent;
	float eps;			///< Epsilon to keep floating point happy and reduce splits.
	
	int initialized;		///< 0 if this BSP needs initialisation, else 1.
	CBspPolyData* polyData;	///< Polygon info structure.
	CVector mins, maxs;		///< Bounding box.
	CSphere sphere;			///< Bounding sphere.
	float volume;			///< Bounding box volume.
	int noSplittingPlane;	///< Set to 1 if getSplitPlane() could not find any suitable splitting plane, else 0.
 
	public:

	/**
	 * Returns the root of the bsp tree. This isn't cashed, so if you need it often, better to store a local copy
	 * of the returned pointer in your own application.
	 */
	CBsp* getRoot()
	{

		CBsp* root = this;

		while (root->parent)
			root = root->parent;

		return root;

	}
	/**
	 * Returns 1 if this BSP node is the root of the BSP tree.
	 */
	int isRoot()
	{

		return parent ? 0 : 1;

	}
	/**
	 * Return true if all polygons lie on one plane.
	 */
	int coplanar();
	/**
	 * Moves a polygon from this BSP to another.
	 * @param poly Polygon to move.
	 * @param bsp Destination BSP.
	 */ 	
	void move(CPoly* poly, CBsp* bsp);
	/**
	 * Return 1 if the polygon lies on the front side of the plane.
	 * @param poly Polygon to test.
	 * @param bias If bias is set to 1, the epsilon value will be doubled.
	 */
	int onFront(CPoly* poly, int bias = 0);
	/**
	 * Return true if the polygon lies on the back side of the plane.
	 * @param poly Polygon to test.
	 * @param bias If bias is set to 1, the epsilon value will be doubled.
	 */
	int onBack(CPoly* poly, int bias = 0);
	/**
	 * Fills in the CBspSplitInfo structure with some statistics on how this plane will split the geometry.
	 * This information is used to decide which plane to use to split.
	 * @param p Splitting plane to get info about.
	 * @param i Info structure that will be filled.
	 * @param flip If set to 1, the plane will be flipped (mirrored).
	 */
	void getSplitInfo(CPlane& p, CBspSplitInfo& i, int flip);
	/**
	 * Returns 1 when i2 is a better split than i1.
	 * @param i1 Info structure 1.
	 * @param i2 Info structure 2.
	 */	
	int test(CBspSplitInfo* i1, CBspSplitInfo* i2);
	/**
	 * Helper function in determining the 'best' splitting plane.
	 * @param i1 Info structure 1.
	 * @param i2 Info structure 2.	 
	 */
	int test1(CBspSplitInfo* i1, CBspSplitInfo* i2);
	/**
	 * Helper function in determining the 'best' splitting plane.
	 * @param i1 Info structure 1.
	 * @param i2 Info structure 2.	 
	 */	
	int test2(CBspSplitInfo* i1, CBspSplitInfo* i2);
	/**
	 * Returns the plane this BSP should be split with. The plane is automatically calculated by using statictical data acquired using test splits.
	 */
	CPlane getSplitPlane();
	/**
	 * Split the polygon, dividing it over the front and back child nodes.
	 * @param poly Polygon to split. This polygon must not be on the front side or back side. It must be spanning.
	 */
	void split(CPoly* poly);
	/**
	 * Splits the BSP node if neccessary.
	 */
	void split();							// Splits / Moves all polys.
	/**
	 * Returns the bounding sphere for a given polygon.
	 * @param poly Polygon for which to calculate bounding sphere.
	 */
	CSphere getBoundingSphere(CPoly* poly);
	void initBoundingSpheres(); ///< Initialises all bounding spheres before initiaiting split.
	void freeBoundingSpheres(); ///< Frees all bounding spheres and restored the polygons' custom data fields.
	void restorePolyData(); ///< Restored the polygons' custom data fields.
	/**
	 * Initializes the bounding box. This is done for every node of the tree.
	 */	   
	void initBoundingBox();
	
	public:
	
	/**
     * Get the leafs the point is in.
     */
    void getHitLeafs(CVector point, std::vector<CBsp*>* bspArray);
    /**
     * Get the leafs the sphere is in.
     */
    void getHitLeafs(CSphere sohere, std::vector<CBsp*>* bspArray);
     

};

#endif
