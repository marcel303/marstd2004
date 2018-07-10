//////////////////////////////////////////////////////////////////////
// CBezier3 implementation.
//////////////////////////////////////////////////////////////////////

CBezier3::CBezier3()
{

	v = 0;
	resolution = 0;

}

CBezier3::~CBezier3()
{

	generate(0);

}

void CBezier3::generate(int a_resolution)
{

	if (v && a_resolution != resolution)
	{
		for (int i=0; i<resolution; i++)
			delete[] v[i];
		delete[] v;
  		v = 0;
		resolution = 0;
	}
		
	if (a_resolution < 2)
		return;
			
	resolution = a_resolution;

	if (!v)
	{
		v = new CVector*[resolution];
		for (int i=0; i<resolution; ++i)
		{
			v[i] = new CVector[resolution];
		}
	}			
 	
	// Interpolate 3rd degree curved surface.
	
	// t + (1-t) = 1
	// (t + (1-t))**3 = 1**3 = 1
	// a = t
	// b = 1-t
	// (a + b)**3 = 1
	// (a*a + 2*a*b + b*b) * (a + b) = 1
	// a*a*a + a*a*b + 2*a*a*b + 2*a*b*b + a*b*b + b*b*b = 1
	// = a*a*a + 3*a*a*b + 3*a*b*b + b*b*b = 1
	// =   A   +    B    +    C    +   D
	// We use A, B, C and D to scale our 4 control points.
	
	DYNAMIC_ARRAY(CVector, tmp[4], resolution, 20);

//	CVector tmp[4][resolution];
		
	// Interpolate vertically.

	for (int i = 0; i < 4; ++i)
	{

		const float step = 1.0 / (resolution - 1.0);
		float t = 0.0;

		for (int j = 0; j < resolution; ++j)
		{

			const float it = 1.0 - t;
			const float a = t * t * t;
			const float b = 3.0 * t * t * it;
			const float c = 3.0 * t * it * it;
			const float d = it * it * it;

			tmp[i][j] = p[i][0] * d + p[i][1] * c + p[i][2] * b + p[i][3] * a;

			t += step;

		}
	}
	
	// Interpolate horizontally.
	for (int i = 0; i < resolution; ++i)
	{

		const float step = 1.0 / (resolution - 1.0);
		float t = 0.0;

		for (int j = 0; j < resolution; ++j)
		{

			const float it = 1.0 - t;
			const float a = t * t * t;
			const float b = 3.0 * t * t * it;
			const float c = 3.0 * t * it * it;
			const float d = it * it * it;

			v[j][i] = tmp[0][i] * d + tmp[1][i] * c + tmp[2][i] * b + tmp[3][i] * a;

			t += step;

		}			
	}
}

void CBezier3::translate(CVector delta)
{

	for (int i = 0; i < 4; ++i)
	{

		for (int j = 0; j < 4; ++j)
		{

			p[i][j] += delta;

		}

	}

}

void CBezier3::toMesh(CMesh& mesh)
{

	if (!v)
       		return;
                
	for (int i = 0; i < resolution - 1; ++i)
	{

       		for (int j = 0; j < resolution - 1; ++j)
		{

                	#define V(_p, _v) \
       	                	{ \
        	                	CEdge* edge = _p->add(new CEdge); \
					edge->p[0] = _v[0]; \
					edge->p[1] = _v[1]; \
					edge->p[2] = _v[2]; \
                                }

			// Triangle 1.

                	CPoly* p1 = mesh.add(new CPoly);

                        V(p1, v[i+0][j+0]);
			V(p1, v[i+1][j+0]);
               	        V(p1, v[i+1][j+1]);

			p1->init();

			// Triangle 2.

			CPoly* p2 = mesh.add(new CPoly);

                        V(p2, v[i+0][j+0]);
       	                V(p2, v[i+1][j+1]);
               	        V(p2, v[i+0][j+1]);

                        p2->init();

			#undef V

                }

	}

}

#undef DYNAMIC_ARRAY

//////////////////////////////////////////////////////////////////////
