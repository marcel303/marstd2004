//////////////////////////////////////////////////////////////////////
// CBsp implementation.
//////////////////////////////////////////////////////////////////////

CBsp::CBsp() : CMesh()
{

	eps = BSP_DEFAULT_EPS;
	
	child[0] = 0;
	child[1] = 0;
	
	parent = 0;
	
	initialized = 0;
	
	polyData = 0;
	
}

CBsp::~CBsp()
{

	if (child[0])
		delete child[0];
	if (child[1])
		delete child[1];
		
}

int CBsp::coplanar()
{

	if (!polyHead || !polyHead->next)
        	return 1;

	plane = polyHead->plane;

	CPoly* p = polyHead->next;

	while (p)
	{

		if (!onFront(p, 1) || !onBack(p, 1))
			return 0;

		p = p->next;
        
	}

	return 1;

}

int CBsp::onFront(CPoly* poly, int bias)
{

	MASSERT(poly);
	
	const float eps2 = bias ? eps * 4.0 : eps;
	
	#if defined(BSP_USE_SPHERES)
	
	const CSphere* sphere = &((CBspPolyData* )poly->data)->sphere;
	if (plane * sphere->position - sphere->radius >= -eps2)
		return 1;
	if (plane * sphere->position + sphere->radius <= +eps2)
		return 0;
		
	#endif   		
	
	CEdge* l = poly->edgeHead;
	
	while (l)
	{
	
		if (plane * l->p <= -eps2)
			return 0;
			
		l = l->next;
		
	}
	
	return 1;
	
}

int CBsp::onBack(CPoly* poly, int bias)
{

	MASSERT(poly);	
	
	const float eps2 = bias ? eps * 4.0 : eps;
	
	#if defined(BSP_USE_SPHERES)		
	
	const CSphere* sphere = &((CBspPolyData* )poly->data)->sphere;
  	// FIXME: We do this test twice. Should make a unified function to classify polygons?
	if (plane * sphere->position + sphere->radius <= +eps2)
		return 1;
	if (plane * sphere->position - sphere->radius >= -eps2)
		return 0;

	#endif

	CEdge* l = poly->edgeHead;
	
	while (l)
	{
	
		if (plane * l->p >= +eps2)
			return 0;
			
		l = l->next;
		
	}
	
	return 1;
	
}

void CBsp::getSplitInfo(CPlane& plane, CBspSplitInfo& i, int flip)
{

	MASSERT(polyCount >= 2);
	
	if (flip)
		i.plane = -plane;
	else
		i.plane = plane;
		
	i.planeFlipped = flip;
       		
	i.fc = 0;
   	i.bc = 0;
	i.dc = 0;
   	i.sc = 0;
       	
	this->plane = i.plane;
        
	CPoly* poly = polyHead;
        
	while (poly)
	{
        
		if (onFront(poly))
			i.fc++;
		else if (onBack(poly))
			i.bc++;
		else
		{
			i.fc++;
			i.bc++;
			i.sc++;
		}
		
		poly = poly->next;
        	
	}
        
	i.dc = i.fc - i.bc;
   	if (i.dc < 0)
		i.dc = -i.dc;

}

CPlane CBsp::getSplitPlane()
{

	MASSERT(polyCount >= 2);
	
	CBspSplitInfo bestSplit, split;

	bestSplit.fc = 0;

	for (CPoly* p = polyHead; p; p = p->next)
	{

		getSplitInfo(p->plane, split, 0);
		if (split.fc > 0 && split.bc > 0 && (test(&split, &bestSplit) || bestSplit.fc == 0))
			bestSplit = split;

		getSplitInfo(p->plane, split, 1);
		if (split.fc > 0 && split.bc > 0 && (test(&split, &bestSplit) || bestSplit.fc == 0))
			bestSplit = split;

	}

	if (bestSplit.fc == 0)
		noSplittingPlane = 1;
	else
		noSplittingPlane = 0;

	#if defined(MARSTD_PARANOID) && defined(BSP_ALTERNATE_CANCEL)

	if (noSplittingPlane)
	{
		marstd_exception(false, 0, "Error: %s: found no splitting plane while 'alternate cancel' is defined", __FUNCTION__);
	}
 			
  	#endif			
  	
	this->planeFlipped = bestSplit.planeFlipped;

	return bestSplit.plane;
	
}

static inline void COPY(CEdge* edge, CPoly* poly)
{

	MASSERT(edge);
	MASSERT(poly);
	
	CEdge* tmp = poly->add(new CEdge);

	*tmp = *edge;

}

static void INTERP(CEdge* l1, CEdge* l2, float t, CPoly* polyFront, CPoly* polyBack)
{

	MASSERT(l1);
	MASSERT(l2);
	MASSERT(polyFront);
	MASSERT(polyBack);
	
	#define DO_INTERP(_n) edgeFront->_n = edgeBack->_n = l1->_n + (l2->_n-l1->_n) * t

	CEdge* edgeFront = polyFront->add(new CEdge);
	CEdge* edgeBack  = polyBack->add(new CEdge);

	DO_INTERP(p[0]);
	DO_INTERP(p[1]);
	DO_INTERP(p[2]);
	DO_INTERP(c[0]);
	DO_INTERP(c[1]);
	DO_INTERP(c[2]);
	DO_INTERP(c[3]);
        
	for (int i = 0; i < VERTEX_TEX_COUNT; ++i)
		for (int j = 0; j < VERTEX_TEX_COMPONENTS; ++j)
			DO_INTERP(t[i][j]);

	#undef DO_INTERP
	
}

void CBsp::split(CPoly* a_p)
{

	MASSERT(a_p);
	
	CPoly* polyFront = child[0]->add(new CPoly);
	CPoly* polyBack  = child[1]->add(new CPoly);
	
	polyFront->data = a_p->data;
	polyBack->data = a_p->data;
        
	CEdge* l1 = a_p->edgeHead;

	while (l1)
	{

		CEdge* l2 = l1->next ? l1->next : a_p->edgeHead;

		float d1 = plane * l1->p;
		float d2 = plane * l2->p;

		const float eps2 = eps * 0.5;
		
		if (d1 >= -eps2 && d1 <= +eps2)
		{
			::COPY(l1, polyFront);
			::COPY(l1, polyBack);
		}
		else if (d1 >= -eps2 && d2 >= -eps2)
		{
			::COPY(l1, polyFront);
		}
		else if (d1 <= +eps2 && d2 <= +eps2)
		{
			::COPY(l1, polyBack);
		}
		else if (d1 >= 0.0 && d2 <= 0.0)
		{
			::COPY(l1, polyFront);
			::INTERP(l1, l2, -d1 / (d2 - d1), polyFront, polyBack);
		}
		else if (d1 <= 0.0 && d2 >= 0.0)
		{
			::COPY(l1, polyBack);
			::INTERP(l1, l2, -d1 / (d2 - d1), polyFront, polyBack);
		}
                
		l1 = l1->next;
        
	}

	// FIXME: Shouldn't really do this. copying normal is OK.

	#if 1

	polyFront->plane = a_p->plane;
	polyBack->plane = a_p->plane;

	polyFront->align();
	polyBack->align();

	#endif

	polyFront->init();
	polyBack->init();

}

#if defined(MARSTD_PARANOID) && 0
static int _d = 0;
class SD { public: SD() { if (::_d == 0) printf("Split depth: "); ::_d++; printf("%d - ", ::_d); } ~SD() { ::_d--; if (::_d == 0) printf("\n"); } };
#endif

void CBsp::split()
{

	#if defined(MARSTD_PARANOID) && 0
	SD _sd;
	#endif
	
	if (child[0])
        	delete child[0];
	if (child[1])
        	delete child[1];

	child[0] = 0;
	child[1] = 0;

	for (CPoly* poly = polyHead; poly; poly = poly->next)
		poly->align();

	if (polyCount <= 1)
	{
		if (polyCount == 1)
			plane = polyHead->plane;	
		return;
	}

	if (!initialized)
	{
		initBoundingSpheres();
		initialized = 1;
	}
        
	initBoundingBox();

	#if 0 // Bail out if polyCount < 10... Just to test how much faster a non 100% BSP would be to generate.
	
	if (poly_count < 10)
	{
		restorePolyData();
		initialized = 0;
		return;
	}
	
	#endif
	
	#if defined(BSP_ALTERNATE_CANCEL)
	
	if (coplanar())
	{
		if (polyCount > 0)
			plane = polyHead->plane;
		restorePolyData();
		initialized = 0;
		return;
	}
        	
	#endif

	plane = getSplitPlane();

	#if !defined(BSP_ALTERNATE_CANCEL)

	if (noSplittingPlane)
	{
		if (polyCount > 0)
			plane = polyHead->plane;
		restorePolyData();
		initialized = 0;
		return;
	}

	#endif
                
	child[0] = new CBsp;
	child[1] = new CBsp;
	
	child[0]->parent = this;
	child[1]->parent = this;
        
	// Children don't need to be initialized.
        
	child[0]->initialized = 1;
	child[1]->initialized = 1;
	
	{
	
		CPoly* p = polyHead;

		while (p)
		{

			CPoly* next = p->next;

			if (onFront(p))
				move(p, child[0]);
			else if (onBack(p))
				move(p, child[1]);
			else
			{
				split(p);
				remove(p);
			}
                
			p = next;
        
		}
        
	}

	child[0]->split();
	child[1]->split();

	freeBoundingSpheres();
	initialized = 0;

}

CSphere CBsp::getBoundingSphere(CPoly* poly)
{

	MASSERT(poly);	
	MASSERT(poly->edgeCount >= 3);
 	
	if (poly->edgeCount < 3)
		return CSphere();
		
	CVector mins, maxs;
	
	mins = poly->edgeHead->p;
	maxs = poly->edgeHead->p;
	
	CEdge* edge = poly->edgeHead->next;
	
	while (edge)
	{
	
		for (int i = 0; i < 3; ++i)
		{
			if (edge->p[i] < mins[i])
				mins[i] = edge->p[i];
			if (edge->p[i] > maxs[i])
				maxs[i] = edge->p[i];
		}
		
		edge = edge->next;
		
	}
	
	CSphere sphere;          			
	sphere.position = (mins + maxs) * 0.5;
	
	edge = poly->edgeHead;
	
	while (edge)
	{
	
		CVector delta = edge->p - sphere.position;
		
		float size2 = delta.size2();
		
		if (size2 > sphere.radius)
			sphere.radius = size2;
			
		edge = edge->next;
		
	}
	
	sphere.radius = sqrt(sphere.radius) + eps * 4.0;
	
	return sphere;
	
}

void CBsp::initBoundingSpheres()
{

	if (polyCount <= 0)
		return;
		
	polyData = new CBspPolyData[polyCount];
	
	CPoly* poly = polyHead;
	
	int index = 0;
	
	while (poly)
	{
	
		polyData[index].data = poly->data;
		polyData[index].sphere = getBoundingSphere(poly);
		
		poly->data = &polyData[index];
		poly = poly->next;
		
		index++;
		
	}
	
}

void CBsp::freeBoundingSpheres()
{

	if (polyData)
		delete[] polyData;
		
}

void CBsp::restorePolyData()
{

	CPoly* poly = polyHead;
	
	while (poly)
	{
	
		MASSERT(poly->data);
		
		CBspPolyData* data = (CBspPolyData*)poly->data;
		poly->data = data->data;
		
		poly = poly->next;
		
	}
	
}

void CBsp::initBoundingBox()
{

	MASSERT(polyHead);
	
	if (!polyHead)
		return;
 
   	// Calculate bounding box.		
	
	MASSERT(polyHead->edgeHead);
	
	mins = polyHead->edgeHead->p;
	maxs = polyHead->edgeHead->p;
	
	CPoly* poly = polyHead;
	
	while (poly)
	{
	
		CEdge* line = poly->edgeHead;
		
		while (line)
		{
		
			for (int i = 0; i < 3; ++i)
			{
				if (line->p[i] < mins[i])
					mins[i] = line->p[i];
				if (line->p[i] > maxs[i])
					maxs[i] = line->p[i];					
			}
   				
			line = line->next;
		
		}
		
		poly = poly->next;
	
	}
	
	// Calculate bounding sphere.
	
	CVector mid;
	int count = 0;
	poly = polyHead;
	
	while (poly)
	{
	
		CEdge* edge = poly->edgeHead;
		
		while (edge)
		{
		
			mid += edge->p;
			count++;
			
			edge = edge->next;
		
		}
		
		poly = poly->next;
	
	}
	
	sphere.position = mid / count;
	
	poly = polyHead;
 
  	while (poly)
	{
  	
  		CEdge* edge = poly->edgeHead;
  		
  		while (edge)
		{
  		
  			CVector delta = sphere.position - edge->p;
  			
  			if (delta.size2() > sphere.radius)
  				sphere.radius = delta.size2();
  				
  			edge = edge->next;
  			
  		}
  		
  		poly = poly->next;
  	
	}
 
  	sphere.radius = sqrt(sphere.radius);	
	
	// Calculate 'volume'.
	
	volume = 1.0;
	for (int i = 0; i < 3; ++i)
		volume *= maxs[i] - mins[i];

}

void CBsp::getHitLeafs(CVector point, std::vector<CBsp*>* bspArray)
{

	MASSERT(bspArray);
	
	if (!child[0] || !child[1])
	{
	
		bspArray->push_back(this);
		
	}
	else
	{

		if (plane * point >= -eps)
		{
			child[0]->getHitLeafs(point, bspArray);
		}
		if (plane * point <= +eps)
		{
			child[1]->getHitLeafs(point, bspArray);
		}
		
	}

}

void CBsp::getHitLeafs(CSphere sphere, std::vector<CBsp*>* bspArray)
{

	MASSERT(bspArray);
	
	if (!child[0] || !child[1])
	{
	
		bspArray->push_back(this);
		
	}
	else
	{

		if (plane * sphere.position >= -sphere.radius - eps)
		{
			child[0]->getHitLeafs(sphere, bspArray);
		}

		if (plane * sphere.position <= +sphere.radius + eps)
		{
			child[1]->getHitLeafs(sphere, bspArray);
		}
		
	}		

}
