//////////////////////////////////////////////////////////////////////
// CVertex implementation.
//////////////////////////////////////////////////////////////////////

CVertex& CVertex::operator=(CVertex vertex)
{

	p = vertex.p;
	
	c[0] = vertex.c[0];
	c[1] = vertex.c[1];
	c[2] = vertex.c[2];
	c[3] = vertex.c[3];
	
	for (int i=0; i<VERTEX_TEX_COUNT; ++i)
		for (int j=0; j<VERTEX_TEX_COMPONENTS; ++j)
			t[i][j] = vertex.t[i][j];
			
	return *this;
	
}   

CVertex CVertex::operator+(CVertex vertex)
{

	CVertex temp;

	temp.p = p + vertex.p;

	for (int i = 0; i < 4; ++i)
		temp.c[i] = c[i] + vertex.c[i];

	for (int i=0; i<VERTEX_TEX_COUNT; ++i)
		for (int j=0; j<VERTEX_TEX_COMPONENTS; ++j)
			temp.t[i][j] = t[i][j] + vertex.t[i][j];

	return temp;

}

CVertex CVertex::operator*(float scale)
{

	CVertex temp;
	
	temp.p = p * scale;

	for (int i = 0; i < 4; ++i)
		temp.c[i] = c[i] * scale;

	for (int i=0; i<VERTEX_TEX_COUNT; ++i)
		for (int j=0; j<VERTEX_TEX_COMPONENTS; ++j)
			temp.t[i][j] = t[i][j] * scale;

	return temp;

}
