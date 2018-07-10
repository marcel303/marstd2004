//////////////////////////////////////////////////////////////////////
// CMatrix3 implementation.
//////////////////////////////////////////////////////////////////////

#include <vector>		// std::swap()

#define INDEX3(x, y) ((x) + (y) * 3)

void CMatrix3::makeIdentity()
{

	v[INDEX3(0, 0)] = 1.0;
	v[INDEX3(1, 0)] = 0.0;
	v[INDEX3(2, 0)] = 0.0;

	v[INDEX3(0, 1)] = 0.0;
	v[INDEX3(1, 1)] = 1.0;
	v[INDEX3(2, 1)] = 0.0;

	v[INDEX3(0, 2)] = 0.0;
	v[INDEX3(1, 2)] = 0.0;
	v[INDEX3(2, 2)] = 1.0;

}

void CMatrix3::transpose()
{

	std::swap(v[INDEX3(1, 0)], v[INDEX3(0, 1)]);
	std::swap(v[INDEX3(2, 0)], v[INDEX3(0, 2)]);
	std::swap(v[INDEX3(2, 1)], v[INDEX3(1, 2)]);

}

void CMatrix3::makeRotation(float x, float y, float z)
{



}

#undef INDEX3

