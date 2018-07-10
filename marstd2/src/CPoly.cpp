//////////////////////////////////////////////////////////////////////
// CPoly implementation.
//////////////////////////////////////////////////////////////////////

#define POLY_EPS 0.001

CPoly::CPoly()
{

	edgeHead = 0;
	edgeTail = 0;
	edgeCount = 0;

	data = 0;

	prev = 0;
	next = 0;

}

CPoly::~CPoly()
{

	clear();
	
	unlink();
	
}

CEdge* CPoly::add(CEdge* edge)
{

	MASSERT(edge);
	
	return addTail(edge);
	
}

CEdge* CPoly::addHead(CEdge* edge)
{

	MASSERT(edge);	
	
	if (edgeHead)
		edge->edge2 = edgeHead;
	else
		edge->edge2 = 0;			
		
	DLLIST_LINK_HEAD(edgeHead, edgeTail, edge, edgeCount);
	
	return edge;
	
}

CEdge* CPoly::addTail(CEdge* edge)
{

	MASSERT(edge); 	
	
	if (edgeTail)
 		edgeTail->edge2 = edge;
	edge->edge2 = edgeHead;
	
	DLLIST_LINK_TAIL(edgeHead, edgeTail, edge, edgeCount);
	
	return edge;
	
}

CEdge* CPoly::unlink(CEdge* edge)
{

	MASSERT(edge); 	
	
	if (edge->prev)
		edge->prev->edge2 = edge->edge2;
	if (edgeHead->edge2 == edge)
		edgeHead->edge2 = edge->edge2;
		
	DLLIST_UNLINK(edgeHead, edgeTail, edge, edgeCount);
	
	return edge;
	
}

void CPoly::remove(CEdge* edge)
{

	MASSERT(edge); 	
	
	if (edge->prev)
		edge->prev->edge2 = edge->edge2;
	if (edgeHead->edge2 == edge)
		edgeHead->edge2 = edge->edge2;		
  	
	DLLIST_REMOVE(edge);
	
}

void CPoly::clear()
{

	DLLIST_CLEAR(edgeHead);
	
}

#define COPY(_edge, tmp) \
{ \
	CEdge* temp__edge = tmp->addTail(new CEdge); \
	temp__edge->p = _edge->p; \
	for (int i = 0; i < 4; ++i) \
		temp__edge->c[i] = _edge->c[i]; \
	for (int i = 0; i < VERTEX_TEX_COUNT; ++i) \
	    for (int j = 0; j < VERTEX_TEX_COMPONENTS; ++j) \
       		temp__edge->t[i][j] = _edge->t[i][j]; \
}

void CPoly::copy(CPoly* poly)
{

	MASSERT(poly);
	
	for (CEdge* edge = edgeHead; edge; edge = edge->next)
		COPY(edge, poly);

	poly->init();

}

CPoly* CPoly::copy()
{

	CPoly* temp = new CPoly;

	copy(temp);

	return temp;

}

void CPoly::init()
{

	MASSERT(edgeHead && edgeHead->next && edgeHead->next->next); 	
	
 	if (!edgeHead || !edgeHead->next || !edgeHead->next->next)
 		return;
 		
	plane.setup(edgeHead->p, edgeHead->next->p, edgeHead->next->next->p);
	plane.normalize();
	
	CEdge* edge1;
  	CEdge* edge2;
  	
  	// edge->plane
  	
    edge1 = edgeHead;
    
	while (edge1)
	{
		edge2 = edge1->next ? edge1->next : edgeHead;
		edge1->edge2 = edge2;
		CVector delta = edge2->p - edge1->p;
		edge1->plane.setup(edge1->p, edge2->p, edge1->p + plane.normal);
		edge1->plane.normalize();
		edge1 = edge1->next;
	}
	
	// edge->edge_plane
	
	edge1 = edgeHead;
	
	while (edge1)
	{
		edge2 = edge1->next ? edge1->next : edgeHead;
		CVector delta = edge2->p - edge1->p;
		float size = delta.size();
		delta /= size;
		edge1->edge_plane.normal = delta;
		edge1->edge_plane.distance = edge1->edge_plane.normal * edge1->p;
		edge1 = edge1->next;
	}
	
	#if defined(MARSTD_PARANOID)
	{
		CVector mid;
		CEdge* edge;
   		edge = edgeHead;
		while (edge)
		{
			mid += edge->p;
			edge = edge->next;
		}
		MASSERT(edgeCount != 0);
		mid /= edgeCount;
		edge = edgeHead;
		while (edge)
		{
			if (edge->plane * mid > 0.0)
			{
				marstd_exception(false, 0, "CPoly::init(): Non convex or non CW polygon.");
			}					
			edge = edge->next;
		}
	}
	#endif
	
}

void CPoly::align()
{

	for (CEdge* edge = edgeHead; edge; edge = edge->next)
	{

		float d1 = plane * edge->p;

		CVector p = edge->p - plane.normal * d1;

		float d2 = plane * p;

		if (d2 < d1)
			edge->p = p;

	}

}

CSphere CPoly::calculateSphere()
{

	MASSERT(edgeCount >= 3);

	CVector mid;
	int count = 0;
	
	CEdge* edge;
  	edge = edgeHead;
  	
	while (edge)
	{
	
		mid += edge->p;
		count++;
		
		edge = edge->next;
		
	}
	
	sphere.position = mid / count;
	
	edge = edgeHead;
	
  	while (edge)
	{
  	
  		CVector delta = sphere.position - edge->p;
  		
  		if (delta.size2() > sphere.radius)
  			sphere.radius = delta.size2();
  			
  		edge = edge->next;
  		
  	}
  	
	sphere.radius = sqrt(sphere.radius);	
	
	return sphere;
	
}

int CPoly::onFront(CPlane* plane)
{

	MASSERT(plane);
	MASSERT(edgeCount >= 3);	
	
	for (CEdge* edge = edgeHead; edge; edge = edge->next)
	{
		float d = (*plane) * edge->p;
		if (d <= -POLY_EPS)
			return 0;
	}
	
	return 1;
	
}

int CPoly::onBack(CPlane* plane)
{

	MASSERT(plane);
	MASSERT(edgeCount >= 3);
	
	for (CEdge* edge = edgeHead; edge; edge = edge->next)
	{
		float d = (*plane) * edge->p;
		if (d >= +POLY_EPS)
			return 0;
	}
	
	return 1;
	
}

#define DO_INTERP(_v) \
	temp__edge->_v = edge1->_v + (edge2->_v - edge1->_v) * t;

#define INTERP(tmp) \
{ \
	const float t = -d1 / (d2 - d1); \
	CEdge* temp__edge = tmp->addTail(new CEdge); \
	DO_INTERP(p); \
	for (int i = 0; i < 4; ++i) \
		DO_INTERP(c[i]); \
		for (int i = 0; i < VERTEX_TEX_COUNT; ++i) \
		    for (int j = 0; j < VERTEX_TEX_COMPONENTS; ++j) \
        		DO_INTERP(t[i][j]); \
}

CPoly* CPoly::clip(CPlane* plane)
{

	MASSERT(plane);
	
	CPoly* tmp = new CPoly;

	if (onFront(plane))
	{
		for (CEdge* edge = edgeHead; edge; edge = edge->next)
			COPY(edge, tmp);
		tmp->init();
		return tmp;
	}

	if (onBack(plane))
		return tmp;

	CEdge* edge1 = edgeHead;

	while (edge1)
	{

		CEdge* edge2 = edge1->edge2;
//		CEdge* edge2 = edge1->next ? edge1->next : edgeHead;

		float d1 = edge1->p * plane->normal - plane->distance;
		float d2 = edge2->p * plane->normal - plane->distance;

		if (d1 >= -POLY_EPS && d1 <= +POLY_EPS)
		{
			COPY(edge1, tmp);
		}
		else if (d1 >= -POLY_EPS && d2 >= -POLY_EPS) // F->F
		{
			COPY(edge1, tmp);
		}
		else if (d1 <= +POLY_EPS && d2 <= +POLY_EPS) // B->B
		{
		}
		else if (d1 >= 0.0 && d2 <= 0.0) // F->B
		{
			COPY(edge1, tmp);
			INTERP(tmp);
		}
		else if (d1 <= 0.0 && d2 >= 0.0) // B->F
		{
			INTERP(tmp);
		}

		edge1 = edge1->next;

	}

	tmp->init();

	return tmp;

}

CPoly* CPoly::clipFront(CPlane* plane)
{

	MASSERT(plane);
	
	CPlane tempPlane = -(*plane);

	return clip(&tempPlane);

}

void CPoly::clip(CPlane* plane, CPoly* front, CPoly* back)
{

	MASSERT(plane);
	MASSERT(front);
	MASSERT(back);
	
	if (onFront(plane))
	{
		copy(front);
		return;
	}
	else if (onBack(plane))
	{
		copy(back);
		return;
	}

	CEdge* edge1 = edgeHead;

	while (edge1)
	{

	    CEdge* edge2 = edge1->edge2;
//		CEdge* edge2 = edge1->next ? edge1->next : edgeHead;

		float d1 = edge1->p * plane->normal - plane->distance;
		float d2 = edge2->p * plane->normal - plane->distance;

//		printf("d1: %f d2: %f\n", d1, d2);

		if (d1 >= -POLY_EPS && d1 <= +POLY_EPS)
		{
			COPY(edge1, front);
			COPY(edge1, back);
		}
		else if (d1 >= -POLY_EPS && d2 >= -POLY_EPS) // F->F
		{
			COPY(edge1, front);
        }
		else if (d1 <= +POLY_EPS && d2 <= +POLY_EPS) // B->B
		{
			COPY(edge1, back);
        }
		else if (d1 >= 0.0 && d2 <= 0.0) // F->B
		{
			COPY(edge1, front);
			INTERP(front);
			INTERP(back);
    	}
		else if (d1 <= 0.0 && d2 >= 0.0) // B->F
		{
			COPY(edge1, back);
			INTERP(back);
			INTERP(front);
        }

        edge1 = edge1->next;

    }

	if (front->edgeCount >= 3 || 1)
	    front->init();
	else
	{
		front->clear();
//		printf("x");
	}
	if (back->edgeCount >= 3 || 1)
	    back->init();
	else
	{
		back->clear();
//		printf("x");
	}

}

#undef INTERP
#undef DO_INTERP

void CPoly::clip(CPoly* poly, CPoly* inside, CMesh* outside)
{

	MASSERT(poly);
	MASSERT(inside);
	MASSERT(outside);
	
	CPoly* tempPoly = new CPoly;
	for (CEdge* edge = edgeHead; edge; edge = edge->next)
	{
		COPY(edge, tempPoly);
	}
	tempPoly->init();

//	printf("tempPoly->edgeCount: %d\n", tempPoly->edgeCount);
//	for (CEdge* edge = tempPoly->edgeHead; edge; edge = edge->next)
//		printf("%f %f %f\n", edge->p[0], edge->p[1], edge->p[2]);

	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
 	{

		CPoly* clip[2];

		clip[0] = new CPoly;
		clip[1] = new CPoly;

//		printf("=== clip ===\n");
//		printf("edge->plane: %f %f %f %f\n", edge->plane.normal[0], edge->plane.normal[1], edge->plane.normal[2], edge->plane.distance);

		tempPoly->clip(&edge->plane, clip[0], clip[1]);

//		printf("tempPoly->edgeCount: %d\n", tempPoly->edgeCount);
//		printf("clip[0]->edgeCount: %d\n", clip[0]->edgeCount);
//		printf("clip[1]->edgeCount: %d\n", clip[1]->edgeCount);

		if (clip[0]->edgeCount >= 3)
		{
			outside->add(clip[0]);
//			printf("outside->add: edgeCount: %d\n", clip[0]->edgeCount);
		}
		else
			delete clip[0];

		delete tempPoly;

		tempPoly = clip[1];

	}

	if (tempPoly->edgeCount >= 3)
	{
		for (CEdge* edge = tempPoly->edgeHead; edge; edge = edge->next)
			COPY(edge, inside);
		inside->init();
	}

	delete tempPoly;

}

void CPoly::reverseWinding()
{

	MASSERT(edgeCount >= 3);
	
	CPoly* tempPoly = new CPoly;
	
	for (CEdge* edge = edgeHead; edge;)
	{
	
		CEdge* next = edge->next;
		
		unlink(edge);
		tempPoly->add(edge);
		
		edge = next;
		
	}
	
	for (CEdge* edge = tempPoly->edgeHead; edge;)
	{
	
		CEdge* next = edge->next;
		
		tempPoly->unlink(edge);
		addHead(edge);
		
		edge = next;
		
	}
	
	init();
	
	delete tempPoly;
 	
}

void CPoly::triangulate(std::vector<CPoly*>* polyArray)
{

	MASSERT(polyArray);
	MASSERT(edgeCount >= 3);
	
	if (edgeCount < 3)
		return;

	int triangleCount = edgeCount - 2;
	
	CEdge* edge1 = edgeHead;
	CEdge* edge2 = edgeHead->next;
	CEdge* edge3 = edgeHead->next->next; 	
	
	for (int i = 0; i < triangleCount; ++i)
	{
	
		CPoly* poly = new CPoly;
		
		COPY(edge1, poly);
		COPY(edge2, poly);
		COPY(edge3, poly);
		
		poly->init();
		
		polyArray->push_back(poly);
    		
		edge2 = edge2->next;
		edge3 = edge3->next;
		
	}

}

#undef INTERP
#undef DO_INTERP
#undef COPY
 	
CPoly* CPoly::unlink()
{

	DLLIST_UNLINK_SELF();
	
}   
                     	
//////////////////////////////////////////////////////////////////////
// CMesh implementation.
//////////////////////////////////////////////////////////////////////

CMesh::CMesh()
{

	polyHead = 0;
	polyTail = 0;
	polyCount = 0;
	
	data = 0;
	
}   

CMesh::~CMesh()
{

	clear();
	
}

CPoly* CMesh::add(CPoly* poly)
{

	MASSERT(poly);
	
 	return addTail(poly);
 	
}

CPoly* CMesh::addHead(CPoly* poly)
{

	MASSERT(poly);	
	
	DLLIST_LINK_HEAD(polyHead, polyTail, poly, polyCount);
	
	return poly;
	
}

CPoly* CMesh::addTail(CPoly* poly)
{

	MASSERT(poly);	
	
	DLLIST_LINK_TAIL(polyHead, polyTail, poly, polyCount);
	
	return poly;
	
} 

CPoly* CMesh::unlink(CPoly* poly)
{

	MASSERT(poly);	
	
	DLLIST_UNLINK(polyHead, polyTail, poly, polyCount);
	
	return poly;
	
}

void CMesh::remove(CPoly* poly)
{

	MASSERT(poly);	
	
	DLLIST_REMOVE(poly);
	
}

void CMesh::clear()
{

	DLLIST_CLEAR(polyHead);
	
}

void CMesh::copy(CMesh* mesh)
{

	MASSERT(mesh);
	
	for (CPoly* poly = polyHead; poly; poly = poly->next)
		mesh->add(poly->copy());

}

CMesh* CMesh::copy()
{

	CMesh* temp = new CMesh;

	copy(temp);

	return temp;

}

void CMesh::triangulate(CMesh* mesh)
{

	MASSERT(mesh);
	
	for (CPoly* poly = polyHead; poly; poly = poly->next)
	{
	
		std::vector<CPoly*> polyArray;
		
		poly->triangulate(&polyArray);
		
		for (unsigned int i = 0; i < polyArray.size(); ++i)
			mesh->add(polyArray[i]);
		
	}
	
}

void CMesh::transform(CMatrix* matrix)
{

	MASSERT(matrix);
	
       	CPoly* poly = polyHead;
       	
        while (poly)
	{
        
               	CEdge* edge = poly->edgeHead;
               	
                while (edge)
		{
                
                       	matrix->apply(edge->p, edge->p);
                       	
                       	edge = edge->next;
                       	
                }
                
               	poly = poly->next;
               	
        }
        
}

void CMesh::move(CMesh* mesh)
{

	MASSERT(mesh);
	
	while (polyHead)
		mesh->add(unlink(polyHead));
		
}

void CMesh::paint(void* data)
{

	CPoly* poly = polyHead;
	
	while (poly)
	{
	
		poly->data = data;
		
		poly = poly->next;
		
	}
	
}

#undef POLY_EPS

//////////////////////////////////////////////////////////////////////
