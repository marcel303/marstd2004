#ifndef __CMatrix_h__
#define __CMatrix_h__

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
/** @file CMatrix.h: CMatrix3, CMatrix4 & CMatrix definition. */
//////////////////////////////////////////////////////////////////////

// FIXME: Use custom matrix functions, not Allegro's ones.
// FIXME: Use degress instead of radians? (OpenGL like).
// FIXME: Use true 4x4 matrix, not allegro hack. Same ordering as OpenGL.

#include "CMath.h"
#include "CVector.h"

//---------------------------------------------------------------------------

#define MATRIX_STACK_SIZE 10 ///< Max matrix stack depth.

//---------------------------------------------------------------------------

/// Geometry: 3x3 matrix.
class CMatrix3
{

	public:

	CMatrix3();
	~CMatrix3();

	public:

	float v[9]; ///< 3x3 matrix data.

	public:

	void makeIdentity(); ///< Creates the identity matrix. Multiplying a vector by the identity matrix results in the same vector.
	void transpose(); ///< Transpose matrix. Swaps rows / columns.
	void makeXRotation(float r); ///< Create a rotation around the X axis. r is given in radians.
	void makeYRotation(float r); ///< Create a rotation around the Y axis. r is given in radians.
	void makeZRotation(float r); ///< Create a rotation around the Z axis. r is given in radians.
	void makeRotation(float x, float y, float z); ///< Create a rotation around the X, Y and Z axis. All rotations are given in radians. The will rotate about the X axis first, then Y axis and finally the Z axis.
	void makeScaling(float x, float y, float z); ///< Creates a scaling matrix.
    void mul(float x, float y, float z, float* xout, float* yout, float* zout); ///< Multiplies a 3D point with this matrix and stores the result in xout, yout and zout.
    CVector mul(CVector& vector, CVector& out); ///< Multiplies a vector by this matrix, storing the result in out.
    void mul(CMatrix3& matrix); ///< Multiplies this matrix bu matrix. This had the effect of concatenating transformations.

	public:

	float determinant(); ///< Calculate the determinant.
	void invert(); ///< Calculate the inverse matrix and replace the current matrix with it.
	
	public:
 
 	float& operator()(int x, int y); ///< Return a value at location (x, y).
 	CVector operator*(CVector& vector); ///< Multiply by vector.
	CMatrix3& operator*=(CMatrix3& matrix); ///< Multiply by matrix. Eg. Matrix3 final = matrix1 * matrix2 * matrix3;.

};

#include "CMatrix4.h"

//---------------------------------------------------------------------------

/// Geometry: Matrix stack.
/**
 * The matrix class is currently just a wrapper for Allegro's matrix functions + stack.
 * Matrices are used to transform geometry (vectors). Matrices can be concatenated
 * to create hierarchical transformations.
 * Both vector and float versions exist of every function to make programming a bit more comfortable.
\code
// Example:

CMatrix matrix;
CVector position, rotation;
CTriangle* triangle;        // Just three points.
int triangles;              // Number of points.

void render_scene()
{

	matrix.push();
	matrix.identity();
	matrix.rotate(rotation);
	matrix.translate(position);
	
	for (int i = 0; i < triangles; ++i) {
		CVector tmp[3];
		for (int j=0; j < 3; ++j)
			matrix.apply(triangle[i][j], tmp[j]);
		render_triangle(tmp);
	}
	
	matrix.pop();

}
\endcode 
 */
 
class CMatrix
{

	public:
 
	CMatrix()
	{
		depth = 0;
		identity();
	} 
	~CMatrix()
	{
	
	}
	
	public:
 
	CMatrix4 m[MATRIX_STACK_SIZE]; ///< Matrix stack.
	int depth; ///< Matrix stack depth.
	CMatrix4 tmp; ///< Temp matrix used by mul()
  	
	public:
 
	/**
     * Pushes the current matrix on the stack.
     * @return The current stack depth.
     * @see pop
     */
	int push()
	{
		m[depth+1] = m[depth];
		depth++;
		return depth;
	} 
 	/**
     * Pops the last pushed matrix from the stack.
     * @return The current stack depth.
     * @see push
     */
	int pop()
	{
		depth--;
		return depth;
	}
	/**
	 * Multiplies the current matrix by tmp.
	 * @see tmp
	 */
	void mul()
	{
		m[depth] *= tmp;
	}
	
	public:
 	
	/**
 	 * Loads the current matrix with the identity matrix.
   	 */ 	
	void identity()
	{
		m[depth].makeIdentity();
	}
	/**
	 * Scales the matrix.
	 * @param x Factor to scale x axis.
	 * @param y Factor to scale y axis.
	 * @param z Factor to scale z axis.  	 
	 */
	void scale(float x, float y, float z)
	{
		tmp.makeScaling(x, y, z);
		mul();
	}
	/**
	 * Scales the matrix using vector[0], vector[1], vector[2].
	 * @param vector Vector with (x, y, z) scaling values.
	 * @see scale()
	 */
	void scale(CVector vector)
	{
		scale(vector[0], vector[1], vector[2]);
	}
	/**
	 * Translates x, y, z along x, y and z axis.
	 * @param x Amount to translate along x axis.
	 * @param y Amount to translate along y axis.
	 * @param z Amount to translate along z axis.  	 
	 */
	void translate(float x, float y, float z)
	{
		tmp.makeTranslation(x, y, z);
		mul();
	}
	/**
	 * Translates by vector[0], vector[1] and vector[2].
	 * @param vector Vector with (x, y, z) translation.
	 * @see translate()
	 */
	void translate(CVector vector)
	{
		translate(vector[0], vector[1], vector[2]);
	}
	/**
	 * Rotates the matrix around the x, y and z axis by specified amounts in radians.
	 * @param x Rotation around the x axis.
	 * @param y Rotation around the y axis.
	 * @param z Rotation around the z axis.  	 	 
	 */	
	void rotate(float x, float y, float z)
	{
		tmp.makeRotation(x, y, z);
		mul();
	}
	/**
	 * @param vector Vector with (x, y, z) rotation.	
	 * @see rotate()
	 */
	void rotate(CVector vector)
	{
		rotate(vector[0], vector[1], vector[2]);
	}
	/**
	 * Scales, rotates and traslates matrix.
	 * @param s* Scaling factors.
	 * @param t* Translation.
	 * @see scale()
	 * @see translate()
	 */
	void st(float sx, float sy, float sz, float tx, float ty, float tz)
	{
		translate(tx, ty, tz);	
		scale(sx, sy, sz);
	}	 
	/**
	 * Clears to rotation part of the matrix. This will still translate to points, but won't rotate them.
	 * Use it to implement billboarding.
	 * @see clear_translation()
	 */
	void clearRotation()
	{
	
	}
	/**
	 * Clears the translation part of the matrix. The new matrix will only rotate points.
	 * Use it to rotate eg. normal vectors.
	 * @see clear_rotation()
	 */
	void clearTranslation()
	{
	
	}
	/**
	 * Applies the current matrix to (x, y, z), storing the result in xout, yout and zout.
	 * @param x X component to transform.
	 * @param y Y component to transform.
	 * @param z Z component to transform.  	 
	 * @param xout Variable to store new x value.
	 * @param yout Variable to store new y value.
	 * @param zout Variable to store new z value.  	 
	 */
	void apply(float x, float y, float z, float& xout, float& yout, float& zout)
	{
		CVector tmp = m[depth] * CVector(x, y, z);
		xout = tmp[0];
		yout = tmp[1];
		zout = tmp[2];  		
	}
	/**
	 * @param v Vector to transform.
	 * @param vout Vector to store result.
	 * @see apply()
	 */
	void apply(CVector& v, CVector& vout)
	{
		vout = m[depth] * v;
	}
 
};

//---------------------------------------------------------------------------

#endif
