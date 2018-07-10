//////////////////////////////////////////////////////////////////////
// CMatrix4 implementation.
//////////////////////////////////////////////////////////////////////

#include <vector>		// std::swap()

#define INDEX4(x, y) ((x) + (y) * 4)

void CMatrix4::makeIdentity()
{

	v[INDEX4(0, 0)] = 1.0;
	v[INDEX4(1, 0)] = 0.0;
	v[INDEX4(2, 0)] = 0.0;
	v[INDEX4(3, 0)] = 0.0;

	v[INDEX4(0, 1)] = 0.0;
	v[INDEX4(1, 1)] = 1.0;
	v[INDEX4(2, 1)] = 0.0;
	v[INDEX4(3, 1)] = 0.0;

	v[INDEX4(0, 2)] = 0.0;
	v[INDEX4(1, 2)] = 0.0;	
	v[INDEX4(2, 2)] = 1.0;
	v[INDEX4(3, 2)] = 0.0;

	v[INDEX4(0, 3)] = 0.0;
	v[INDEX4(1, 3)] = 0.0;
	v[INDEX4(2, 3)] = 0.0;
	v[INDEX4(3, 3)] = 1.0;
	
}

void CMatrix4::transpose()
{

	std::swap(v[INDEX4(1, 0)], v[INDEX4(0, 1)]);
	std::swap(v[INDEX4(2, 0)], v[INDEX4(0, 2)]);
	std::swap(v[INDEX4(3, 0)], v[INDEX4(0, 3)]);
	std::swap(v[INDEX4(2, 1)], v[INDEX4(1, 2)]);
	std::swap(v[INDEX4(3, 1)], v[INDEX4(1, 3)]);
	std::swap(v[INDEX4(3, 2)], v[INDEX4(2, 3)]);

}

void CMatrix4::makeXRotation(float r)
{

	makeIdentity();

	float sinx = sin(r);
	float cosx = cos(r);

	v[INDEX4(1, 1)] = cosx;
	v[INDEX4(1, 2)] = -sinx;

	v[INDEX4(2, 1)] = sinx;
	v[INDEX4(2, 2)] = cosx;

}

void CMatrix4::makeYRotation(float r)
{

	makeIdentity();

	float siny = sin(r);
	float cosy = cos(r);

	v[INDEX4(0, 0)] = cosy;
	v[INDEX4(0, 2)] = -siny;

	v[INDEX4(2, 0)] = siny;
	v[INDEX4(2, 2)] = cosy;

}

void CMatrix4::makeZRotation(float r)
{

	makeIdentity();

	float sinz = sin(r);
	float cosz = cos(r);

	v[INDEX4(0, 0)] = cosz;
	v[INDEX4(0, 1)] = sinz;

	v[INDEX4(1, 0)] = -sinz;
	v[INDEX4(1, 1)] = cosz;

}

void CMatrix4::makeRotation(float x, float y, float z)
{

	makeIdentity();

	// FIXME: Rotate -x, because OpenGL does this? It's abit hackish, but makes matrix rotation compatible with OpenGL. :|

	makeXRotation(-x);
//	makeXRotation(x);

	CMatrix4 temp;

	temp.makeYRotation(y);
	mul(temp);

	temp.makeZRotation(z);
	mul(temp);


}

void CMatrix4::makeTranslation(float x, float y, float z)
{

	makeIdentity();

	v[INDEX4(3, 0)] = x;
	v[INDEX4(3, 1)] = y;
	v[INDEX4(3, 2)] = z;

}

void CMatrix4::makeScaling(float x, float y, float z)
{

	makeIdentity();

	v[INDEX4(0, 0)] = x;
	v[INDEX4(1, 1)] = y;
	v[INDEX4(2, 2)] = z;

}

#undef INDEX4

