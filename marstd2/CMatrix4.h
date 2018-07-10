#ifndef __CMatrix4_h__
#define __CMatrix4_h__

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
/** @file CMatrix4.h: CMatrix4. */
//////////////////////////////////////////////////////////////////////

/// Geometry: 4x4 matrix.
/**
 * NOTE: 3D vectors are padded with 1.0. So eg the 3D vector (1.0, 2.0, 3.0) becomes the 4D vector (1.0, 2.0, 3.0, 1.0). This makes multiplication by a 4x4 matrix possible and adds the translation part.
 */
class CMatrix4
{

	public:
 
	CMatrix4();
 	~CMatrix4();
  
	public:
 
	float v[16];

	public:
 
	void makeIdentity();
 	void transpose(); ///< Swaps (X, Y) position of the values. Usefull when converting from OpenGL <-> Direct3D eg.
  	void makeXRotation(float r); ///< Create a rotation around the X-axis.
  	void makeYRotation(float r); ///< Create a rotation around the Y-axis.
  	void makeZRotation(float r); ///< Create a rotation around the Z-axis.
  	void makeRotation(float x, float y, float z); ///< Create a rotation around all three axis. Rotates around X, Y and Z axis in that order.
   	void makeScaling(float x, float y, float z); ///< Create a scaling matrix.
    void makeTranslation(float x, float y, float z); ///< Create a translation matrix.
    void mul(float x, float y, float z, float w, float* xout, float* yout, float* zout, float* wout); ///< Multiply the 4D vector with the matrix and store the result in xout, yout, zout and wout.
    void mul4x3(float x, float y, float z, float* xout, float* yout, float* zout); ///< Multiply the 3D vector with the matrix, padding the vector with 1.0.
    void mul3x3(float x, float y, float z, float* xout, float* yout, float* zout); ///< Multiply the 3D vector with the matrix, without padding the vector with 1.0.
    void mul(CVector& vector, CVector& out); ///< Multiply the 4D vector with the matrix and store the result in xout, yout, zout and wout.
    void mul4x3(CVector& vector, CVector& out); ///< Multiply the 3D vector with the matrix, padding the vector with 1.0.
    void mul3x3(CVector& vector, CVector& out); ///< Multiply the 3D vector with the matrix, without padding the vector with 1.0.
    void mul(CMatrix4& matrix); ///< Multiply the matrix with matrix and store the result in this matrix.
    void mul3x3(CMatrix3& matrix); ///< Multiply the 3x3 part of the matrix with matrix and store the result in this matrix.
    /**
     * Multiply the 3x3 part of the matrix with matrix as if it were a 4x3 matrix and store the result in this matrix.
     * Now some more details on how this works...
\code
The 3x3 matrix matrix,

( X1Y1  X2Y1  X3Y1 )

( X1Y2  X2Y2  X3Y2 )

( X1Y3  X2Y3  X3Y3 )

is expanded to the 4x4 matrix:

( X1Y1  X2Y1  X3Y1  0.0 )

( X1Y2  X2Y2  X3Y2  0.0 )

( X1Y3  X2Y3  X3Y3  0.0 )

( 0.0   0.0   0.0   1.0 )
\endcode
    * Thus when using 3x3 matrices to represent rotations and 4x4 matrices to represent transformation, eg rotations + translation, this operations has the same effect as multipling the matrix by a 4x4 matrix with the translation zeroed out.
    */
    void mul4x3(CMatrix3& matrix);
    void mul(CMatrix4& matrix, CMatrix4& out); ///< Multiply the matrix with matrix and store the result in out. This method assumes matrix != out && this != out.
    void mul3x3(CMatrix3& matrix, CMatrix4& out); ///< Multiply the 3x3 part of the matrix with matrix and store the result in out. This method assumes matrix != out && this != out.
    void mul4x3(CMatrix3& matrix, CMatrix4& out); ///< Multiply the 3x3 part of the matrix with matrix as if it were a 4x3 matrix and store the result in out. This method assumes matrix != out && this != out. @see mul3x3
    
	public:

	float determinant();
	void invert();

	public:

	float operator()(int x, int y);         ///< Retrieve a value of the matrix by using the function operator.
	CVector operator*(CVector vector);     ///< Multiply the 3D vector with the matrix and return the result.
	CMatrix4 operator*(CMatrix4& matrix);   ///< Mutiply the matrix with matrix and return a reference to this matrix.
	CMatrix4 operator*(CMatrix3& matrix);   ///< Mutiply the matrix with matrix and return a reference to this matrix.
	CMatrix4& operator*=(CMatrix4& matrix); ///< Mutiply the matrix with matrix and return a reference to this matrix.
	CMatrix4& operator*=(CMatrix3& matrix); ///< Mutiply the matrix with matrix and return a reference to this matrix.

};

// TODO: look for optimizations in Allegro source code.

class CMatrix4Stack
{

	public:
	
	CMatrix4Stack();
	~CMatrix4Stack();
	
	public:
	
	std::vector<CMatrix4> stack; ///< Stack of matrices. This is basically a vector that stores objects of the Matrix4 type.

	CMatrix4 matrix; ///< "Current" matrix.
	
	public:
	
	void push(); ///< Pushes the current matrix onto the stack.
	void pop(); ///< Restores the matrix from the last matrix pushed onto the stack.

};

//---------------------------------------------------------------------------

#endif

