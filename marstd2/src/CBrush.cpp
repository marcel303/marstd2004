//////////////////////////////////////////////////////////////////////
// CBrush implementation.
//////////////////////////////////////////////////////////////////////

CBrush::CBrush()
{

	planeHead = 0;
	planeTail = 0;
	planeCount = 0;
	
	data = 0;
	
	prev = 0;
	next = 0;
	
}

CBrush::~CBrush()
{

	clear();
	
	unlink();
	
}

CPlane* CBrush::add(CPlane* plane)
{

	return addTail(plane);
	
}

CPlane* CBrush::addHead(CPlane* plane)
{

	DLLIST_LINK_HEAD(planeHead, planeTail, plane, planeCount);
	
	return plane;
	
}

CPlane* CBrush::addTail(CPlane* plane)
{

	DLLIST_LINK_TAIL(planeHead, planeTail, plane, planeCount);
	
	return plane;
	
}

void CBrush::remove(CPlane* plane)
{

	DLLIST_REMOVE(plane);
	
}

CPlane* CBrush::unlink(CPlane* plane)
{

	DLLIST_UNLINK(planeHead, planeTail, plane, planeCount);
	
	return plane;
	
}

void CBrush::clear()
{

	DLLIST_CLEAR(planeHead);
	
}

CPoly* CBrush::initial(CPlane* plane)
{

	float max = -1.0;
	int x = -1;
	
	for (int i = 0; i < 3; ++i)
	{
	
		float v = fabs(plane->normal[i]);
		
		if (v > max)
		{
			x = i;
			max = v;
		}
		
	}
	
	CVector vup;
        
	if (x < 2)
		vup[2] = 1.0;
	else
		vup[0] = 1.0;
               	
	// Origin of our poly, a point on the plane.
	
	CVector org = plane->normal * plane->distance;
        
	// Get translation vectors.
	
	float v = vup * plane->normal;
	vup = vup.MA(plane->normal, -v);
	vup.normalize();
        
	CVector vright = vup % plane->normal;
        
	const float huge = 65536.0;

	// Make our initial polygon 'infinitely' large.

	vup *= huge;
	vright *= huge;
        
	CPoly* tmp = new CPoly;
        
	// Add four corner points.
	
	CEdge* edge[4];
       	
	for (int i = 0; i < 4; ++i)
		edge[i] = tmp->add(new CEdge);
	        
	edge[0]->p = org - vright + vup;
	edge[1]->p = org + vright + vup;
	edge[2]->p = org + vright - vup;
	edge[3]->p = org - vright - vup;

	tmp->init();

	return tmp;

}

CPoly* CBrush::poly(CPlane* plane)
{

	CPoly* poly = initial(plane);
	CPlane* plane2 = planeHead;
	
	while (plane2 && poly)
	{
        
		if (plane2 != plane)
		{
        	
			CPoly* poly2 = poly->clip(plane2);

			delete poly;
               	        
			poly = poly2;
                       	
		}
                
		plane2 = plane2->next;

	}
        
	return poly;
        
}

CMesh* CBrush::mesh()
{

	CMesh* mesh2 = new CMesh;
	CPlane* plane = planeHead;

	while (plane)
	{

		CPoly* poly2 = poly(plane);

		if (poly2)
			mesh2->add(poly2);

		plane = plane->next;

	}
        
	return mesh2;

}

CEdge* CBrush::points()
{

	CMesh* mesh2 = mesh();
	
	CEdge* point = 0;
	CPoly* poly = mesh2->polyHead;
       	
	while (poly)
	{
        
		CEdge* edge = poly->edgeHead;
       		
		while (edge)
		{
       		
			CEdge* edge2 = new CEdge;
			*edge2 = *edge;
			edge2->next = point;
               	        
			if (point)
				point->prev = edge2;
       	                	
			point = edge2;
			
			edge = edge->next;
                	
		}
       	        
		poly = poly->next;
       	        
	}
        
	delete mesh2;
       	
	return point;
        
}

CBrush* CBrush::unlink()
{

	DLLIST_UNLINK_SELF();
	
}

//////////////////////////////////////////////////////////////////////
