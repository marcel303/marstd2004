/// Geometry: Primitives: Cube generator.
/**
 * Creates an orthogonal cube with corners (-1.0, -1.0, -1.0), (+1.0, +1.0, +1.0).
 * The generated polygons will be stored in the specified mesh.
 * @param mesh Mesh to put generated polygon in.
 */
void CGeomBuilder::cube(CMesh& mesh)
{

	const int vertice[8][3] =
        {
            { -1, -1, -1 },
            { +1, -1, -1 },
            { +1, +1, -1 },
            { -1, +1, -1 },
            { -1, -1, +1 },
            { +1, -1, +1 },
            { +1, +1, +1 },
            { -1, +1, +1 }
        };

	const int face[6][4] =
        {
            { 3, 2, 1, 0 },	// OK
            { 4, 5, 6, 7 }, // OK
            { 0, 4, 7, 3 },	// OK
            { 2, 6, 5, 1 },	// OK
            { 0, 1, 5, 4 },	// OK
            { 7, 6, 2, 3 }	// OK
        };

	for (int i = 0; i < 6; ++i)
	{

		CPoly* poly = mesh.add(new CPoly);

		for (int j=0; j<4; j++)
		{

			CEdge* edge = poly->add(new CEdge);

			edge->p[0] = vertice[face[i][j]][0];
			edge->p[1] = vertice[face[i][j]][1];
			edge->p[2] = vertice[face[i][j]][2];
                
		}

		transform(poly);

		poly->init();
        
	}

}
