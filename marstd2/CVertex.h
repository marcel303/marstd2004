#ifndef __cVERTEX_h__
#define __cVERTEX_h__

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
/** @file CVertex.h: CVertex implementation. */
//////////////////////////////////////////////////////////////////////

#include <vector>

#define VERTEX_TEX_COUNT      2 ///< The maximum number of texture coordinates per vertex. Must at least be 1.
#define VERTEX_TEX_COMPONENTS 3 ///< The number of texture components. Must at least be 3.

//---------------------------------------------------------------------------

/// Geometry: Vertex type.
/**
 * The vertex type is used to represent vertices for 3D models which can be rendered by eg. OpenGL.
 * The CVertex class is not derivaed from CVector because the operator[] is hard to access when
 * dealing with pointers to CVertex structures. Eg: (*vertex)[0] = 0.0 is IMHO less appealing than
 * vertex->p[0] = 0.0.
 */
class CVertex
{

    public:
 
    CVertex();
    ~CVertex();

    public:

    CVector p;  ///< XYZ position.
	float c[4]; ///< RGBA colour. Range [0.0, 1.0].
    float t[VERTEX_TEX_COUNT][VERTEX_TEX_COMPONENTS]; ///< UVST texture coordinates. Range [0.0, 1.0].

	public:
    
	/**
	 * Sets four colour components. Alpha is defined as 1.0 by default.
	 */
	void setColour(float r, float g, float b, float a=1.0);
	/**
	 * Sets texture coordinates for texture 0. Third texture coordinate defaults to 0.0.
	 */
	void setTexCoord(float u, float v, float w=0.0);
	/**
	 * Sets texture coordinates for texture texture.
	 * @param texture Index of texture coordinates to set. Range [0..VERTEX_TEX_COUNT-1].
	 * @see set_texture()
	 */
	void setTexCoord(int texture, float u, float v, float w=0.0);
	
	public:
	
	CVertex interpolateLinear(CVertex& vertex1, CVertex& vertex2, float t);

    public:

	/**
	 * Copies vertex.
	 */
	CVertex& operator=(CVertex vertex);
	/**
     * Scales all components of vertex.
     * @param scale Scaling factor to multiply all components with.
     * @return Vertex with value of all components of vertex scaled by scale
     */
    CVertex operator*(float scale);
	/**
     * Adds all components of vertex.
     * @param vertex Vertex to add to vertex object
     * @return Vertex with value of all components of vertices summed.
     */
    CVertex operator+(CVertex vertex);

};

//---------------------------------------------------------------------------

typedef std::vector<CVertex> CVertexArray;        ///< STL CVertex vector.
typedef CVertexArray::iterator CVertexArrayItr;   ///< STL CVertex vector iterator.

typedef std::vector<CVertex*> CPVertexArray;      ///< STL CVertex* vector.
typedef CPVertexArray::iterator CPVertexArrayItr; ///< STL CVertex* vector iterator.

//---------------------------------------------------------------------------

#endif
