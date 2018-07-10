#ifndef __CIsosurface_h__
#define __CIsosurface_h__

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
/** @file CIsosurface.h: CIsosurface implementation. */
//////////////////////////////////////////////////////////////////////

#include "CPoly.h"

/// Geometry: Iso surface: Grid value.
typedef struct
{
	float p[3];    ///< Position. Range [-1.0..+1.0].
	float n[3]; ///< Normal vector.
	float e;          ///< Energy value.
	int normalized;   ///< Has the normal been calculated / normalized?
} CIsosurfaceValue;

/// Geometry: Iso surface: Triangle vertex.
typedef struct
{
	float p[3];			///< Position. Range [-1.0..+1.0].
	float n[3];		///< Normal vector.
} CIsosurfaceVertex;

// Geometry: Iso surface: Cube definition.
typedef struct
{
	CIsosurfaceValue* v[8];		///< 8 corner points of cube.
} CIsosurfaceCube;

/// Geometry: Iso surface: Metaball.
class CIsosurfaceMetaball
{

	public:

	CIsosurfaceMetaball()
	{
	
	}
	~CIsosurfaceMetaball()
	{
	
	}

	public:

	float p[3];		///< XYZ Position.
	float a;		///< Energy. May be negative too.

};

/// Geometry: Iso surface class.
/**
 * Use this class to visualize iso surfaces. Support has been added to calculate energy values for metaballs.
 * The calculation of the generated triangles is done using the marching cubes algoritm.
 * This algoritms is patented, so be carefull using it in a commercial product!<BR>
\code
// Example:

void render() {

	load_camera();
	
	// Create a medium sized iso surface.
	static CIsosurface iso;
	iso.setSize(10, 10, 10);

	// Create a metaball.
	CIsosurfaceMetaball ball;
	ball.p[0] = sin(time);
	ball.p[1] = 0.0;
	ball.p[2] = cos(time);
	ball.a = 1.0;

	iso.calculate(&ball, 1);		// Calculate energy values using metaballs.
	iso.calculateNormals();		// Calculate normals.

	// Output triangles.	
	
	CIsosurfaceVertex v[300];
	glVertexPointer(GL_FLOAT, 3, v, sizeof(CIsosurfaceVertex));
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, v, sizeof(CIsosurfaceVertex));
	glEnableClientState(GL_VERTEX_ARRAY);	

	glBegin(GL_TRIANGLES); {	
		iso.output(100, v, render_triangle);
	} glEnd();
	
}

void render_triangle(int triangles, CIsosurfaceVertex* v) {
	glDrawArrays(0, triangles*3);
}
\endcode
 */
class CIsosurface
{

	public:

	CIsosurface();
	~CIsosurface();

	public:

	CIsosurfaceValue*** v;		///< Energy values.
	CIsosurfaceCube*** cube;	///< Cube for marching cubes algo.
	int sx, sy, sz;			///< Size of the grid.
	float treshold;			///< Isosurface border. If energy(x, y, z) >= treshold, (x, y, z) is solid.

	protected:
	
	CIsosurfaceValue* vLinear;
 	CIsosurfaceCube* cubeLinear;
  	
	public:

	/**
	 * Sets the size of the grid. Passing (0, 0, 0) will free memory.
	 * @param sx Resolution in x direction.
	 * @param sy Resolution in y direction.
	 * @param sz Resolution in z direction.  	 
	 */
	void setSize(int sx, int sy, int sz);

	public:

	/**
	 * Calculates energy values for a given number of mataballs.
	 * Note that you don't have to use this function to generate energy values.
	 * You can use all kinds of custom functions to do this.
	 * @param ball Pointer to first ball in array.
	 * @param balls The size of the ball array.
	 */
	void calculate(CIsosurfaceMetaball* ball, int balls);
	/**
	 * Call this just before output() to have valid normal values if you need them.
	 */
	void calculateNormals();
	/**
	 * Spawns triangles. The callback function will be called for every triangle with
	 * three metavert_t structures.
	 * @param triangle Callback function that will be called for every generated triangle.
	 */
	void output(void (*triangle)(CIsosurfaceVertex* v));
	/**
	 * Spawns multiple triangles at once. Triangles is the maximum number of triangles
         * to spawn at once. v must be an array of triangles*3 metavert_t structures.
         * The callback function will be called with the number of triangles to render and
         * a pointer to v. Triangles are stored 123.123.123.
         * @param triangles The maximum number of triangles to output at once.
         * @param v An array of vertices large enough to store triangles*3 vertices.
         * @param triangle Callback function that will be called for multiple generated triangles.
	 */
	void output(int triangles, CIsosurfaceVertex* v, void (*triangle)(int num, CIsosurfaceVertex* v));
	/**
	 * Spawn triangles to a mesh. This method isn't recommended for real-time use.
	 * @param mesh Mesh to output to.
	 */
	void output(CMesh& mesh);

};

#endif
