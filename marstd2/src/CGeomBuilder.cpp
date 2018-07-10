//////////////////////////////////////////////////////////////////////
// CGeomBuilder implementation.
//////////////////////////////////////////////////////////////////////

void CGeomBuilder::transform(CPoly* poly)
{

	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
	{

		matrix.apply(edge->p, edge->p);

	}

}

#include "CGeomBuilder/Cilinder.cpp"
#include "CGeomBuilder/Circle.cpp"
#include "CGeomBuilder/Cone.cpp"
#include "CGeomBuilder/Cube.cpp"
#include "CGeomBuilder/Donut.cpp"
#include "CGeomBuilder/Sphere.cpp"

//////////////////////////////////////////////////////////////////////
