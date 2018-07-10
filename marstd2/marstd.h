#ifndef __marstd_h__
#define __marstd_h__

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
/** @file marstd.h: MarSTD main include file. */
//////////////////////////////////////////////////////////////////////

/**
\mainpage MarSTD 2004 documentation
<TT>
//////////////////////////////////////////////////////////////////////<BR>
// MarSTD version 2004 - (c)2004 - Marcel Smit                        <BR>
//                                                                    <BR>
// Marcel_Smit_101@hotmail.com                                        <BR>
// marcel.athlon@hccnet.nl                                            <BR>
//                                                                    <BR>
// This code may not be used in a commercial product without my       <BR>
// permission. If you redistribute it, this message must remain       <BR>
// intact. If you use this code, some acknowledgement would be        <BR>
// appreciated. ;-)                                                   <BR>
//////////////////////////////////////////////////////////////////////<BR>
</TT>
<BR>
MarSTD is a librairy written by Marcel Smit that provides some basic classes to
define and manipulate geometry. It also includes some math functions, geometry
generators, geometry compilers, a general purpose file class, and more.<BR>
<BR>
Main features:
- Many geometric primitives.
- Advanced geometric operations like CSG and BSP tree construction.
- Floating point stability.
- Ease of use.
- Quite robust and stable if I may say so. ;)

Take a look at the examples for a good introduction to the librairy. Use these docs as a reference to fill in the gaps.
The classes are all meant to be easy and natural to use.<BR>
IMHO documentation generated from source code is generally quite poor and not a good starting point to learn use a librairy at all.
So again, please take a look at the example code first.
*/

//#define MARSTD_3DNOW		///< Uncomment to use 3DNow! instructions. TODO: Implement 3DNow!.

// DEBUG CODE
#include "CDebug.h"

// NEW CODE
#include "CList.h"          // dll: helper macros
#include "CDynArray.h"      // stack: helper macros
#include "CVector.h"        // geometry: vector
#include "CVertex.h"        // geometry: vertex
#include "CSphere.h"        // geometry: sphere
#include "CPoly.h"          // geometry: polygon, mesh
#include "CMatrix.h"        // math: 3x3 matrix and stack
#include "CMatrix4.h"       // math: 4x4 matrix and stack
#include "CMath.h"          // math: angle conversions, etc
#include "CIsosurface.h"    // geometry: metaballs
#include "CFile.h"          // io: file
#include "CGeomBuilder.h"   // geometry: primitives builder
#include "CGeomCompiler.h"  // geometry: geometry compiler
#include "CBezier.h"		// geometry: bezier patches
#include "CCsg2D.h"         // geometry: 2D csg
#include "CCsg3D.h"         // geometry: 3D csg

// OLD CODE. DON'T USE! (YET)
#include "CBsp.h"		// bsp operations
#include "CBrush.h"		// geometry: brushes
//#include "COctree.h"	// geometry: octree

#include "inline/CVertex.inl"
#include "inline/CPoly.inl"
//#include "inline/COctree.inl"
#include "inline/CMatrix.inl"
#include "inline/CMatrix4.inl"
#include "inline/CIsosurface.inl"
#include "inline/CGeomCompiler.inl"
#include "inline/CGeomBuilder.inl"
#include "inline/CBsp.inl"
#include "inline/CBrush.inl"
#include "inline/CBezier.inl"
#include "inline/CCsg2D.inl"
#include "inline/CCsg3D.inl"

#endif
