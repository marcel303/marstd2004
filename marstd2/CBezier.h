#ifndef __Cbezier_h__
#define __Cbezier_h__

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
/** @file CBezier.h: CBezier implementation. */
//////////////////////////////////////////////////////////////////////

#include "CVector.h"
#include "CPoly.h"

//---------------------------------------------------------------------------

/// Geometry: 3rd degree bezier patch generator class.
/**
 * The bezier patch class uses a 3rd degree function to calculate a patch
 * using 4x4 control points. The patch will touch the four outer control points. The rest
 * of the control points will affect the shape of the patch.
\code
// Example:

void add_bezier(CBsp& bsp) {

	CBezier3 bezier;
	
	// Set corner control points.
	
	bezier.p[0][0].set(-1.0, -1.0, -1.0);
	bezier.p[3][0].set(+1.0, -1.0, -1.0);
	bezier.p[3][3].set(+1.0, +1.0, -1.0);
	bezier.p[0][3].set(-1.0, +1.0, -1.0);
	
	// Generate 10x10 patch.
	
	bezier.generate(10);
	
	// Export to mesh. Remember that CBsp is derived from CMesh.
	
	bezier.to_mesh(bsp);

}
\endcode 
 */
class CBezier3
{

	public:
 
	/**
     * Constructor.
     */
	CBezier3();
	/**
	 * Destructor.
	 */
 	~CBezier3();
  
	public:
 
	CVector p[4][4];	///< 4x4 grid of control points.
	
	public:
 	
	CVector** v;		///< Output grid points.
	int resolution;		///< Resolution of output grid.
	
 public:
 
	/**
     * Generates the output grid. The grid will be resolution*resolution sized.
     * If resolution <= 0, all memory will be freed instead.
     * @param resolution The resolution of the generated patch. The generated patch will have resolution*resolution points.
     */
	void generate(int a_resolution = 7);
	/**
	 * Translates all control points by the specified vector. You will need to regenerate
	 * to update the output grid.
	 * @param delta The delta vector will be added to all 4x4 control points.
	 */
	void translate(CVector delta);

 public:

	/**
	 * Outputs the generated points as triangles.
	 * @param mesh Target mesh to output triangles.
	 */
	void toMesh(CMesh& mesh);
        
};

//---------------------------------------------------------------------------

#endif
