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
/** @file cBSP.h: CCompiledBsp implementation. */
//////////////////////////////////////////////////////////////////////

#include "CGeomCompiler.h"

typedef struct
{

	CPlane plane; ///< Splitting plane.

	int fc; ///< Number of polygons in front of plane after split.
	int bc; ///< Number of polygons behind plane after split.
	int dc; ///< Difference of front and back polys.
	int sc; ///< Number of polygons spanning plane.

} CCompiledBspSplitInfo;

typedef struct
{

	void* data;			///< Data pointer of poly.
	CSphere sphere;		///< Bubble around the origional polygon.
	
} CCompiledBspPolyData;

/// Geometry: BSP class.
/**
 * @author Marcel Smit
 */
class CCompiledBsp : public CCompiledMesh
{

	public:

	CCompiledBsp();
	~CCompiledBsp();

	public:

 	CPlane plane; ///< Splitting plane.
 	CCompiledBsp* child[2]; ///< Front & back children

	CCompiledMesh* root; ///< Base mesh where vertex, edge, etc data is stored.

	public:

	/**
	 * Compiles the bsp tree into a compiled bsp structure.
	 * @param bsp BSP tree to compile.
	 */
	void compile(CBsp* bsp);

	protected:

	void getLeafNodes(CBsp* bsp, std::vector<CBsp*>& bspArray);
	void getLeafNodes(std::vector<CCompiledBsp*>& cbspArray);
	void replicateHierarchy(CBsp* bsp); ///< Creates a hierarchy structure that resembles the tree structure of bsp.

	public:

	/**
     * Get the leafs the point is in.
     */
    void getHitLeafs(CVector& point, std::vector<CCompiledBsp*>& bspArray);
    /**
     * Get the leafs the sphere is in.
     */
    void getHitLeafs(CSphere& sohere, std::vector<CCompiledBsp*>& bspArray);

};

#endif
