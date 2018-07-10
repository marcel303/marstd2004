/// Geometry: Primitives: Circle generator.
/**
 * Creates a circle in the OXY plane, with radius 1.0 and slices corner points.
 * The higher the number of slices, the better the approximation of a true circle will be.
 * The generated polygons will be stored in the specified mesh.
 * @param mesh Mesh to put generated polygon in.
 * @param slices Number of divisions. More division make to shape look smoother.
 */
void CGeomBuilder::circle(CMesh& mesh, int sides)
{

	float a = 0.0;
	float s = 2.0 * M_PI / sides;

	CPoly* poly = mesh.add(new CPoly);
        
	for (int i = 0; i < sides; ++i)
	{

		CEdge* edge = poly->add(new CEdge);

		edge->p[0] = sin(a);
		edge->p[1] = cos(a);

		a += s;
        
	}

	transform(poly);

	poly->init();

}
