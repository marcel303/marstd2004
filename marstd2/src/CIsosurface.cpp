//////////////////////////////////////////////////////////////////////
// CIsosurface implementation.
//////////////////////////////////////////////////////////////////////

CIsosurface::CIsosurface()
{

	v = 0;
	cube = 0;
     
    vLinear = 0;
    cubeLinear = 0;
       
	sx = sy = sz = 0;
       	
	treshold = 1.0;	
        
}

CIsosurface::~CIsosurface()
{

	setSize(0, 0, 0);
	
}

void CIsosurface::setSize(int a_sx, int a_sy, int a_sz)
{

	// FIXME: Single allocations. Eg, x = new float[sx * sy * sz].

	if (vLinear)
 		delete[] vLinear;
   
    vLinear = 0;
    
    if (cubeLinear)
    	delete[] cubeLinear;
    	
	cubeLinear = 0;
    	
	if (v)
	{

		for (int i = 0; i < sx; ++i)
		{
//			for (int j = 0; j < sy; ++j)
//				delete[] v[i][j];
			delete[] v[i];
		}

		delete[] v;

		v = 0;

		for (int i = 0; i < sx - 1; ++i)
		{
//			for (int j = 0; j < sy - 1; ++j)
//				delete[] cube[i][j];
			delete[] cube[i];
		}

		delete[] cube;

		cube = 0;

		sx = sy = sz = 0;

	}

	if (a_sx <= 0 || a_sy <= 0 || a_sz <= 0)
		return;

	sx = a_sx;
	sy = a_sy;
	sz = a_sz;

	vLinear = new CIsosurfaceValue[sx * sy * sz];
	cubeLinear = new CIsosurfaceCube[sx * sy * sz];
 	
	// Create and setup grid.
        
	v = new CIsosurfaceValue**[sx];
	for (int i = 0; i < sx; ++i)
	{
		v[i] = new CIsosurfaceValue*[sy];
		const float x = -1 + 2 * i / (sx - 1.0);
		for (int j = 0; j < sy; ++j)
		{
//			v[i][j] = new CIsosurfaceValue[sz];
			v[i][j] = &vLinear[i * sy * sz + j * sz];
			const float y = -1 + 2 * j / (sy - 1.0);
			for (int k = 0; k < sz; ++k)
			{
				const float z = -1 + 2 * k / (sz - 1.0);
				v[i][j][k].p[0] = x;
				v[i][j][k].p[1] = y;
				v[i][j][k].p[2] = z;
				v[i][j][k].n[0] = 0.0;
				v[i][j][k].n[1] = 0.0;
				v[i][j][k].n[2] = 0.0;
				v[i][j][k].normalized = 1;
			}
		}
	}

	// Create and setup cubes.
        
	cube = new CIsosurfaceCube**[sx - 1];
    
	for (int i = 0; i < sx - 1; ++i)
	{
    
		cube[i] = new CIsosurfaceCube*[sy - 1];
    	
		for (int j = 0; j < sy - 1; ++j)
		{
//			cube[i][j] = new CIsosurfaceCube[sz - 1];
			cube[i][j] = &cubeLinear[i * sy * sz + j * sz];
			for (int k = 0; k < sz - 1; ++k)
			{
                        
				// Point to the eight shared corner vertices.
                                
				#define P(_p, _x, _y, _z) cube[i][j][k].v[_p] = &v[i + _x][j + _y][k + _z]
                                
				P(0, 0, 0, 0);
				P(1, 1, 0, 0);
				P(2, 1, 0, 1);
				P(3, 0, 0, 1);
				P(4, 0, 1, 0);
				P(5, 1, 1, 0);
				P(6, 1, 1, 1);
				P(7, 0, 1, 1);
                                
				#undef P
                                
			}
		}
	}

}

void CIsosurface::calculate(CIsosurfaceMetaball* ball, int balls)
{

	// Calculate max distance for a metaball to be added into the calculation:
	//
	// ball.a / d2 < treshold / C
	// ball.a * C / d2 < treshold
	// d2 < treshold * ball.a * C

	float treshold2[balls];

	for (int i = 0; i < balls; ++i)
	{
		treshold2[i] = treshold * ball[i].a * 5;
	}
	
	// Calculate energy values.

    for (int i = 0; i < sx; ++i)
    	for (int j = 0; j < sy; ++j)
        	for (int k = 0; k < sz; ++k)
         	{

            	// FIXME: This could be optimized quite a bit.

            	float e = 0.0;
                                
            	for (int b = 0; b < balls; ++b)
				{

                	float dx = v[i][j][k].p[0] - ball[b].p[0];
                	float dy = v[i][j][k].p[1] - ball[b].p[1];
                	float dz = v[i][j][k].p[2] - ball[b].p[2];

			float d2 = dx * dx + dy * dy + dz * dz;

			// E = A / (R*R)
                                        
			if (d2 == 0.0)
				e += treshold;
			else
			{
				/*if (d2 < treshold2[b])*/
					e += ball[b].a / d2;
			}
                                
		}

		v[i][j][k].e = e;
                        
	}
                
}

void CIsosurface::calculateNormals()
{

	// Calculate normals. We do this for every value in the grid. Maybe should do it only
	// for used points when outputting triangles.
	// FIXME: Interpolating normals linearly changes size.
	//	  Size of normals of neightbouring grid values is more or less the same when not normalized...
	//	  Maybe normalize when outputting?
	//    Normalizing when outputting gives unpleasant results at borders.
 
	for (int i = 1; i < sx - 1; ++i)
 		for (int j = 1; j < sy - 1; ++j)
   			for (int k = 1; k < sz - 1; ++k)
			{
				#if 1
				// Default method.
   				v[i][j][k].n[0] = v[i-1][j][k].e - v[i+1][j][k].e;
				v[i][j][k].n[1] = v[i][j-1][k].e - v[i][j+1][k].e;
				v[i][j][k].n[2] = v[i][j][k-1].e - v[i][j][k+1].e;
				#else
				// Experimental method.
   				v[i][j][k].n[0] = v[i][j][k].e - v[i+1][j][k].e;
   				v[i][j][k].n[1] = v[i][j][k].e - v[i][j+1][k].e;
   				v[i][j][k].n[2] = v[i][j][k].e - v[i][j][k+1].e;
   				#endif
				v[i][j][k].normalized = 0;
			}
			
}

static void (*cb_triangle)(CIsosurfaceVertex* v);

static void cb_triangles(int num, CIsosurfaceVertex* v)
{

	for (int i = 0; i < num; ++i)
	{

		// Triangle per triangle output.
    
		cb_triangle(v);
            
		v += 3;
            
	}
	
}

void CIsosurface::output(void (*triangle)(CIsosurfaceVertex* v))
{

	::cb_triangle = triangle;
        
	CIsosurfaceVertex v[300];

	output(100, v, ::cb_triangles);

}

static float _isosurface_treshold = 1.0;

static inline void _isosurface_normalize(CIsosurfaceValue* p)
{
	float is = 1.0 / sqrt(p->n[0] * p->n[0] + p->n[1] * p->n[1] + p->n[2] * p->n[2]);
	p->n[0] *= is;
	p->n[1] *= is;
	p->n[2] *= is;
	p->normalized = 1;
}

static inline void _isosurface_interpolate(CIsosurfaceValue* p1, CIsosurfaceValue* p2, CIsosurfaceVertex* pout)
{

	if (!p1->normalized)
		_isosurface_normalize(p1);
	if (!p2->normalized)
		_isosurface_normalize(p2);
		
    if (p1->e == p2->e || p1->e == _isosurface_treshold)
    {

		// Copy first point.
		
		pout->p[0] = p1->p[0];
    	pout->p[1] = p1->p[1];
    	pout->p[2] = p1->p[2];
    	pout->n[0] = p1->n[0];
    	pout->n[1] = p1->n[1];
    	pout->n[2] = p1->n[2];
        
    }
    else if (p2->e == _isosurface_treshold)
    {

		// Copy second point.
		
		pout->p[0] = p2->p[0];
    	pout->p[1] = p2->p[1];
    	pout->p[2] = p2->p[2];
    	pout->n[0] = p2->n[0];
    	pout->n[1] = p2->n[1];
    	pout->n[2] = p2->n[2];

    }
    else
    {
        
		// Interpolate linearly 99% of time. Note this is probably slightly incorrect, but fast.
        	
		// e1 + de * t = d
		// t = (d - e1) / de
        	
		const float t = (_isosurface_treshold - p1->e) / (p2->e - p1->e);

		pout->p[0] = p1->p[0] + t * (p2->p[0] - p1->p[0]);
		pout->p[1] = p1->p[1] + t * (p2->p[1] - p1->p[1]);
		pout->p[2] = p1->p[2] + t * (p2->p[2] - p1->p[2]);
		pout->n[0] = p1->n[0] + t * (p2->n[0] - p1->n[0]);
		pout->n[1] = p1->n[1] + t * (p2->n[1] - p1->n[1]);
		pout->n[2] = p1->n[2] + t * (p2->n[2] - p1->n[2]);

    }
    
}

#include "CIsosurface.inc"

/*

This functions does most of the magic. It uses the marching cubes algoritms
to find a nice surface for the energy values in the grid. If you want to
read some texts explaining visualisation of iso surfaces, search the net
for "marching cubes", "meta balls", "iso surfaces" and "implicit surfaces".

*/

void CIsosurface::output(int triangles, CIsosurfaceVertex* v, void (*triangle)(int num, CIsosurfaceVertex* p))
{

        ::_isosurface_treshold = treshold;

        int current_triangle = 0;

        for (int i = 0; i < sx - 1; ++i)
        	for (int j = 0; j < sy - 1; ++j)
                	for (int k = 0; k< sz - 1; ++k)
                 	{

	// ** VERY ** SPECIAL CASE! I DON'T INDENT HERE!

	CIsosurfaceCube* g = &this->cube[i][j][k];

	CIsosurfaceVertex p[12];

	// Get cube index. Every cube point could be in or outside
	// treshold. With 8 corners, this is 2**8 = 256 possibilities.
	// 256 different cubes, and we got all there information stored in
	// an array! hah!

	int cube_index = 0;

	if (g->v[0]->e < treshold) cube_index |= 1;
	if (g->v[1]->e < treshold) cube_index |= 2;
	if (g->v[2]->e < treshold) cube_index |= 4;
	if (g->v[3]->e < treshold) cube_index |= 8;
	if (g->v[4]->e < treshold) cube_index |= 16;
	if (g->v[5]->e < treshold) cube_index |= 32;
	if (g->v[6]->e < treshold) cube_index |= 64;
	if (g->v[7]->e < treshold) cube_index |= 128;

	// edges tell us which edges to interpolate. Do nothing if there are none.

	const int edges = edge_table[cube_index];

	if (edges == 0)
		continue;

	// Check edge table for this cube to know which corners to interpolate between, and which not.

	if (edges & 1)		_isosurface_interpolate(g->v[0], g->v[1], &p[0]);	// "Top"
	if (edges & 2)		_isosurface_interpolate(g->v[1], g->v[2], &p[1]);
	if (edges & 4)		_isosurface_interpolate(g->v[2], g->v[3], &p[2]);
	if (edges & 8)		_isosurface_interpolate(g->v[3], g->v[0], &p[3]);
	if (edges & 16)		_isosurface_interpolate(g->v[4], g->v[5], &p[4]);	// "Bottom"
	if (edges & 32)		_isosurface_interpolate(g->v[5], g->v[6], &p[5]);
	if (edges & 64)		_isosurface_interpolate(g->v[6], g->v[7], &p[6]);
	if (edges & 128)	_isosurface_interpolate(g->v[7], g->v[4], &p[7]);
	if (edges & 256)	_isosurface_interpolate(g->v[0], g->v[4], &p[8]);	// "Left"
	if (edges & 512)	_isosurface_interpolate(g->v[1], g->v[5], &p[9]);
	if (edges & 1024)	_isosurface_interpolate(g->v[2], g->v[6], &p[10]);	// "Right"
	if (edges & 2048)	_isosurface_interpolate(g->v[3], g->v[7], &p[11]);

	int index = 0;

	while (triangle_table[cube_index][index] != -1) 
	{

		// Some copying here. Cannot be prevented if we want to output the way we do.
		
		v[current_triangle * 3 + 0] = p[triangle_table[cube_index][index + 0]];
		v[current_triangle * 3 + 1] = p[triangle_table[cube_index][index + 1]];
		v[current_triangle * 3 + 2] = p[triangle_table[cube_index][index + 2]];

		current_triangle++;

		// Output if buffer is full.
		
		if (current_triangle == triangles)
		{
			triangle(triangles, v);
			current_triangle = 0;
		}

		index += 3;

	}

			}

	if (current_triangle)
		triangle(current_triangle, v);

}

static CMesh* _isosurface_output_mesh;

static void _isosurface_cb_output_to_mesh(CIsosurfaceVertex* v)
{

	CPoly* poly = _isosurface_output_mesh->add(new CPoly);
	
	for (int i = 0; i < 3; ++i)
	{
	
		CEdge* edge = poly->add(new CEdge);
		
		edge->p[0] = v[i].p[0];
		edge->p[1] = v[i].p[1];
		edge->p[2] = v[i].p[2];  		
		
	}
 
   	poly->init();		

}

void CIsosurface::output(CMesh& mesh)
{

	::_isosurface_output_mesh = &mesh;
	
	output(_isosurface_cb_output_to_mesh);

}

