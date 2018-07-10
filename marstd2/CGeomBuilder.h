#ifndef __cGEOMBUILDER_h__
#define __cGEOMBUILDER_h__

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
/** @file CVector.h: CVector implementation. */
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "cDEBUG.h"
#include "cMATRIX.h"

//---------------------------------------------------------------------------

/// Geometry: Geometry builder object.
/**
 * The geometry builder provides methods to dynamically generate geometry like cubes, spheres and cilinders.
 * All generated geometry is transformed by the geometry builder's matrix. Unlike other
 * classes the geometry builder cannot be instantiated by the programmer. The geometry builder class is a so
 * called singleton, which means only one instance exists of which a reference is acquired by called the I() method.
\code
// Example:

CMesh mesh;

CGeomBuilder::I().matrix.push();
{
	// Create a (4, 6, 2) sized box with center at (0, 0, 0).
	CGeomBuilder::I().matrix.identity();
	CGeomBuilder::I().matrix.scale(2.0, 3.0, 1.0);
	CGeomBuilder::I().cube(mesh);
}
CGeomBuilder::I().matrix.pop();
\endcode
 */
class CGeomBuilder {

	private:

    CGeomBuilder()
    {
    
    }
    ~CGeomBuilder()
    {
    
    }
    
	public:
    
	/**
     * Get interface method. Call this to acquire an instace of the geometry builder object.
     */
    static CGeomBuilder& I()
    {
        static CGeomBuilder geomBuilder;
        return geomBuilder;
    }
    
	public:
 
    CMatrix matrix; ///< Generates geometric primitives are transformed by this matrix before being outputted.
    
	public:

	/**
     * Generate a cube from (-1, -1, -1) to (1, 1, 1).
     */
    void cube(CMesh& mesh);
    /**
     * Circles are at z = -1 and z = +1
     * Cilinder is oriented along the z axis.
     * @param sides The number of sides the cilinder has. Higher means better silhouette.
     */
    void cilinder(CMesh& mesh, int sides);
    /**
     * Generate a circle with radius 1, in the XY plane.
     * @param sides The number of edges.
     */
    void circle(CMesh& mesh, int sides);
    /**
     * Generate a cone shape. Cone converges in (0, 0, 1).
     * @param sides Subdivision factor.
     */
    void cone(CMesh& mesh, int sides);
    /**
     * Generate a sphere with radius 1.
     * @param div1 Subdivisions along the z axis.
     * @param div2 Subdivisions in the XY plane.
     */
    void sphere(CMesh& mesh, int div1, int div2);

    void donut(CMesh& mesh, int sides, int slices, float r1, float r2);
    
    private:
    
    void transform(CPoly* poly);

};

#endif
