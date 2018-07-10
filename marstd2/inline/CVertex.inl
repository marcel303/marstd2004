//////////////////////////////////////////////////////////////////////
// CVertex implementation.
//////////////////////////////////////////////////////////////////////

inline CVertex::CVertex()
{

}

inline CVertex::~CVertex()
{

}

inline void CVertex::setColour(float r, float g, float b, float a)
{

	c[0] = r;
	c[1] = g;
	c[2] = b;
	c[3] = a;
	
}

inline void CVertex::setTexCoord(float u, float v, float w)
{

	t[0][0] = u;
	t[0][1] = v;
	t[0][2] = w;
	
}

inline void CVertex::setTexCoord(int texture, float u, float v, float w)
{

	t[texture][0] = u;
	t[texture][1] = v;
	t[texture][2] = w;
	
}

inline CVertex interpolateLinear(CVertex& vertex1, CVertex& vertex2, float t)
{

	CVertex temp = vertex1 * (1.0 - t) + vertex2 * t;
	
	return temp;

}
