//////////////////////////////////////////////////////////////////////
// CMatrix4 implementation.
//////////////////////////////////////////////////////////////////////

#include <vector>		// std::swap()

#define INDEX3(x, y) ((x) + (y) * 3)
#define INDEX4(x, y) ((x) + (y) * 4)

inline CMatrix4::CMatrix4()
{

}

inline CMatrix4::~CMatrix4()
{

}

inline void CMatrix4::mul(float x, float y, float z, float w, float* xout, float* yout, float* zout, float* wout)
{

	*xout =
		v[INDEX4(0, 0)] * x +
		v[INDEX4(1, 0)] * y +
		v[INDEX4(2, 0)] * z +
		v[INDEX4(3, 0)] * w;

	*yout =
		v[INDEX4(0, 1)] * x +
		v[INDEX4(1, 1)] * y +
		v[INDEX4(2, 1)] * z +
		v[INDEX4(3, 1)] * w;

	*zout =
		v[INDEX4(0, 2)] * x +
		v[INDEX4(1, 2)] * y +
		v[INDEX4(2, 2)] * z +
		v[INDEX4(3, 2)] * w;

	*wout =
		v[INDEX4(0, 3)] * x +
		v[INDEX4(1, 3)] * y +
		v[INDEX4(2, 3)] * z +
		v[INDEX4(3, 3)] * w;

}

inline void CMatrix4::mul4x3(float x, float y, float z, float* xout, float* yout, float* zout) {

    // Assume w = 1.0.
    
	*xout =
		v[INDEX4(0, 0)] * x +
		v[INDEX4(1, 0)] * y +
		v[INDEX4(2, 0)] * z +
		v[INDEX4(3, 0)];

	*yout =
		v[INDEX4(0, 1)] * x +
		v[INDEX4(1, 1)] * y +
		v[INDEX4(2, 1)] * z +
		v[INDEX4(3, 1)];

	*zout =
		v[INDEX4(0, 2)] * x +
		v[INDEX4(1, 2)] * y +
		v[INDEX4(2, 2)] * z +
		v[INDEX4(3, 2)];

}

inline void CMatrix4::mul3x3(float x, float y, float z, float* xout, float* yout, float* zout)
{

	// Assume w = 0.0.
    
	*xout =
		v[INDEX4(0, 0)] * x +
		v[INDEX4(1, 0)] * y +
		v[INDEX4(2, 0)] * z;

	*yout =
		v[INDEX4(0, 1)] * x +
		v[INDEX4(1, 1)] * y +
		v[INDEX4(2, 1)] * z;

	*zout =
		v[INDEX4(0, 2)] * x +
		v[INDEX4(1, 2)] * y +
		v[INDEX4(2, 2)] * z;

}

inline void CMatrix4::mul(CVector& vector, CVector& out)
{

	mul(vector[0], vector[1], vector[2], vector[3], &out[0], &out[1], &out[2], &out[3]);

}

inline void CMatrix4::mul4x3(CVector& vector, CVector& out)
{

	mul4x3(vector[0], vector[1], vector[2], &out[0], &out[1], &out[2]);
	
}

inline void CMatrix4::mul3x3(CVector& vector, CVector& out)
{

	mul3x3(vector[0], vector[1], vector[2], &out[0], &out[1], &out[2]);
	
}
    
inline void CMatrix4::mul(CMatrix4& matrix)
{

    float tmp[16];

    for (int x = 0; x < 4; ++x)
    {
        for (int y = 0; y < 4; ++y)
        {

            tmp[INDEX4(x, y)] =
                v[INDEX4(0, y)] * matrix.v[INDEX4(x, 0)] +
                v[INDEX4(1, y)] * matrix.v[INDEX4(x, 1)] +
                v[INDEX4(2, y)] * matrix.v[INDEX4(x, 2)] +
                v[INDEX4(3, y)] * matrix.v[INDEX4(x, 3)];

        }
    }

    for (int i = 0; i < 16; ++i)
        v[i] = tmp[i];

}

inline void CMatrix4::mul3x3(CMatrix3& matrix)
{

    float tmp[9];

    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {

            tmp[INDEX3(x, y)] =
                v[INDEX4(0, y)] * matrix.v[INDEX3(x, 0)] +
                v[INDEX4(1, y)] * matrix.v[INDEX3(x, 1)] +
                v[INDEX4(2, y)] * matrix.v[INDEX3(x, 2)];

        }
    }

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            v[INDEX4(i, j)] = tmp[INDEX3(i, j)];

}

inline void CMatrix4::mul4x3(CMatrix3& matrix)
{

    float tmp[16];

    for (int x = 0; x < 4; ++x)
    {
        for (int y = 0; y < 4; ++y)
        {

            if (x == 3)
            {
                tmp[INDEX4(x, y)] =
                    v[INDEX4(0, y)] * matrix.v[INDEX3(x, 0)] +
                    v[INDEX4(1, y)] * matrix.v[INDEX3(x, 1)] +
                    v[INDEX4(2, y)] * matrix.v[INDEX3(x, 2)];
            }
            else
            {
                tmp[INDEX4(x, y)] =
                    v[INDEX4(0, y)] +
                    v[INDEX4(1, y)] +
                    v[INDEX4(2, y)];
            }

        }
    }

    for (int i = 0; i < 16; ++i)
        v[i] = tmp[i];

}

inline void CMatrix4::mul(CMatrix4& matrix, CMatrix4& out)
{

    for (int x = 0; x < 4; ++x)
    {
        for (int y = 0; y < 4; ++y)
        {

            out.v[INDEX4(x, y)] =
                v[INDEX4(0, y)] * matrix.v[INDEX4(x, 0)] +
                v[INDEX4(1, y)] * matrix.v[INDEX4(x, 1)] +
                v[INDEX4(2, y)] * matrix.v[INDEX4(x, 2)] +
                v[INDEX4(3, y)] * matrix.v[INDEX4(x, 3)];

        }
    }

}

inline void CMatrix4::mul3x3(CMatrix3& matrix, CMatrix4& out)
{

    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {

            out.v[INDEX4(x, y)] =
                v[INDEX4(0, y)] * matrix.v[INDEX3(x, 0)] +
                v[INDEX4(1, y)] * matrix.v[INDEX3(x, 1)] +
                v[INDEX4(2, y)] * matrix.v[INDEX3(x, 2)];

        }
    }

    // Copy right column.

    out.v[INDEX4(3, 0)] = v[INDEX4(3, 0)];
    out.v[INDEX4(3, 1)] = v[INDEX4(3, 1)];
    out.v[INDEX4(3, 2)] = v[INDEX4(3, 2)];

    // Copy bottom row.

    out.v[INDEX4(0, 3)] = v[INDEX4(0, 3)];
    out.v[INDEX4(1, 3)] = v[INDEX4(1, 3)];
    out.v[INDEX4(2, 3)] = v[INDEX4(2, 3)];
    out.v[INDEX4(3, 3)] = v[INDEX4(3, 3)];


}

inline void CMatrix4::mul4x3(CMatrix3& matrix, CMatrix4& out)
{

    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {

            if (x == 3)
            {
                out.v[INDEX4(x, y)] =
                    v[INDEX4(0, y)] * matrix.v[INDEX3(x, 0)] +
                    v[INDEX4(1, y)] * matrix.v[INDEX3(x, 1)] +
                    v[INDEX4(2, y)] * matrix.v[INDEX3(x, 2)];
            }
            else
            {
                out.v[INDEX4(x, y)] =
                    v[INDEX4(0, y)] +
                    v[INDEX4(1, y)] +
                    v[INDEX4(2, y)];
            }
        }
    }

    // Copy right column.

    out.v[INDEX4(3, 0)] = v[INDEX4(3, 0)];
    out.v[INDEX4(3, 1)] = v[INDEX4(3, 1)];
    out.v[INDEX4(3, 2)] = v[INDEX4(3, 2)];

    // Copy bottom row.

    out.v[INDEX4(0, 3)] = v[INDEX4(0, 3)];
    out.v[INDEX4(1, 3)] = v[INDEX4(1, 3)];
    out.v[INDEX4(2, 3)] = v[INDEX4(2, 3)];
    out.v[INDEX4(3, 3)] = v[INDEX4(3, 3)];


}

inline float CMatrix4::operator()(int x, int y)
{

	return v[INDEX4(x, y)];

}

inline CVector CMatrix4::operator*(CVector vector)
{

	CVector tmp;

	tmp[0] =
		v[INDEX4(0, 0)] * vector[0] +
		v[INDEX4(1, 0)] * vector[1] +
		v[INDEX4(2, 0)] * vector[2] +
		v[INDEX4(3, 0)]; // * 1.0

	tmp[1] =
		v[INDEX4(0, 1)] * vector[0] +
		v[INDEX4(1, 1)] * vector[1] +
		v[INDEX4(2, 1)] * vector[2] +
		v[INDEX4(3, 1)]; // * 1.0

	tmp[2] =
		v[INDEX4(0, 2)] * vector[0] +
		v[INDEX4(1, 2)] * vector[1] +
		v[INDEX4(2, 2)] * vector[2] +
		v[INDEX4(3, 2)]; // * 1.0

	// tmp[3] = 1.0;

	return tmp;

}

inline CMatrix4 CMatrix4::operator*(CMatrix4& matrix)
{

	CMatrix4 tmp = *this;
	tmp *= matrix;
	return tmp;

}

inline CMatrix4 CMatrix4::operator*(CMatrix3& matrix)
{

	CMatrix4 tmp = *this;
	tmp *= matrix;
	return tmp;
	
}
	
inline CMatrix4& CMatrix4::operator*=(CMatrix4& matrix)
{

	mul(matrix);

	return *this;

}

inline CMatrix4& CMatrix4::operator*=(CMatrix3& matrix)
{

	mul4x3(matrix);

	return *this;

}

#undef INDEX3
#undef INDEX4

