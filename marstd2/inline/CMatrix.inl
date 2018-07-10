//////////////////////////////////////////////////////////////////////
// CMatrix implementation.
//////////////////////////////////////////////////////////////////////

#include <vector>		// std::swap()

#define INDEX3(x, y) ((x) + (y) * 3)

inline CMatrix3::CMatrix3()
{

}

inline CMatrix3::~CMatrix3()
{

}

inline void CMatrix3::mul(float x, float y, float z, float* xout, float* yout, float* zout)
{

    *xout =
        v[INDEX3(0, 0)] * x +
        v[INDEX3(1, 0)] * y +
        v[INDEX3(2, 0)] * z;
        
    *yout =
        v[INDEX3(0, 1)] * x +
        v[INDEX3(1, 1)] * y +
        v[INDEX3(2, 1)] * z;
        
    *zout =
        v[INDEX3(0, 2)] * x +
        v[INDEX3(1, 2)] * y +
        v[INDEX3(2, 2)] * z;

}

inline void CMatrix3::mul(CMatrix3& matrix)
{

    float tmp[9];
    
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
        
            tmp[INDEX3(x, y)] =
                v[INDEX3(0, y)] * matrix.v[INDEX3(x, 0)] +
                v[INDEX3(1, y)] * matrix.v[INDEX3(x, 1)] +
                v[INDEX3(2, y)] * matrix.v[INDEX3(x, 2)];

        }
    }
    
    for (int i = 0; i < 9; ++i)
        v[i] = tmp[i];

}

inline float CMatrix3::determinant()
{

	// Calculate determinant using Sarrus' method.

	const float d11 = v[INDEX3(0, 0)] * v[INDEX3(1, 1)] * v[INDEX3(2, 2)];
	const float d12 = v[INDEX3(1, 0)] * v[INDEX3(2, 1)] * v[INDEX3(0, 2)];
	const float d13 = v[INDEX3(2, 0)] * v[INDEX3(0, 1)] * v[INDEX3(1, 2)];

	const float d1 = d11 + d12 + d13;

	const float d21 = v[INDEX3(2, 0)] * v[INDEX3(1, 1)] * v[INDEX3(0, 2)];
	const float d22 = v[INDEX3(1, 0)] * v[INDEX3(0, 1)] * v[INDEX3(2, 2)];
	const float d23 = v[INDEX3(0, 0)] * v[INDEX3(2, 1)] * v[INDEX3(1, 2)];

	const float d2 = d21 + d22 + d23;

	const float d = d1 - d2;

	return d;

}

inline float& CMatrix3::operator()(int x, int y)
{

	return v[INDEX3(x, y)];

}

inline CVector CMatrix3::operator*(CVector& vector)
{

	CVector tmp;

	tmp[0] =
		v[INDEX3(0, 0)] * vector[0] +
		v[INDEX3(1, 0)] * vector[1] +
		v[INDEX3(2, 0)] * vector[2];

	tmp[1] =
		v[INDEX3(0, 1)] * vector[0] +
		v[INDEX3(1, 1)] * vector[1] +
		v[INDEX3(2, 1)] * vector[2];

	tmp[2] =
		v[INDEX3(0, 2)] * vector[0] +
		v[INDEX3(1, 2)] * vector[1] +
		v[INDEX3(2, 2)] * vector[2];

	return tmp;

}

inline CMatrix3& CMatrix3::operator*=(CMatrix3& matrix)
{

    mul(matrix);

	return *this;

}

#undef INDEX3

