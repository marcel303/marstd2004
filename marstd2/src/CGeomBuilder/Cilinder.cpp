/// Geometry: Primitives: Cilinder generator.
/**
 * Creates a mesh around the z axis with radius 1.0 and height from -1.0 to +1.0.
 * The higher the number of slices, the better the approximation of a true cilinder will be.
 * The generated polygons will be stored in the specified mesh.
 * @param mesh Mesh to put generated polygon in.
 * @param slices Number of divisions. More divisions make the shape look smoother.
 */
void CGeomBuilder::cilinder(CMesh& mesh, int slices)
{

	float s = 2.0 * M_PI / slices;
	float a = 0.0;

	CPoly* cb = mesh.add(new CPoly);
	CPoly* ct = mesh.add(new CPoly);	
	
        for (int i=0; i < slices; ++i)
	{

		// Side.
		
		CPoly* poly = mesh.add(new CPoly);

		CEdge* edge1 = poly->add(new CEdge); edge1->p.set(sin(a  ),   cos(a  ),   +1.0);
		CEdge* edge2 = poly->add(new CEdge); edge2->p.set(sin(a + s), cos(a + s), +1.0);
		CEdge* edge3 = poly->add(new CEdge); edge3->p.set(sin(a + s), cos(a + s), -1.0);
		CEdge* edge4 = poly->add(new CEdge); edge4->p.set(sin(a  ),   cos(a  ),   -1.0);

		transform(poly);

		poly->init();

		// Top and bottom.
                
		CEdge* edge;
                
		edge = cb->add(new CEdge);
		edge->p.set(sin(a), cos(a), -1.0);
                
		edge = ct->add(new CEdge);
		edge->p.set(sin(-a), cos(-a), +1.0);
                
		a += s;

	}
	
	transform(cb);
	transform(ct);

	cb->init();
	ct->init();

}
