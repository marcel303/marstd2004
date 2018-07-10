//////////////////////////////////////////////////////////////////////
// CCsg2D implementation.
//////////////////////////////////////////////////////////////////////

CCsg2D::CCsg2D()
{

}

CCsg2D::~CCsg2D()
{

}

void CCsg2D::addition(CPoly* poly1, CPoly* poly2, CMesh* out)
{

	CPoly* tempPoly = new CPoly;

	poly2->clip(poly1, tempPoly, out);

	out->add(poly1->copy());

	delete tempPoly;

}

void CCsg2D::subtraction(CPoly* poly1, CPoly* poly2, CMesh* out)
{

	CPoly* tempPoly = new CPoly;

	poly1->clip(poly2, tempPoly, out);

	delete tempPoly;

}

void CCsg2D::intersection(CPoly* poly1, CPoly* poly2, CPoly* out)
{

	CPoly* tempPoly = new CPoly;

	CMesh tempMesh;

	poly1->clip(poly2, tempPoly, &tempMesh);

	if (tempPoly->edgeCount >= 3)
		tempPoly->copy(out);

	delete tempPoly;

}

void CCsg2D::exclusiveor(CPoly* poly1, CPoly* poly2, CMesh* out)
{

	CPoly* tempPoly = new CPoly;

	poly1->clip(poly2, tempPoly, out);

	tempPoly->clear();

	poly2->clip(poly1, tempPoly, out);

	delete tempPoly;

}

//////////////////////////////////////////////////////////////////////
