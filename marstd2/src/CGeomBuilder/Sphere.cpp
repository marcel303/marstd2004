#include <math.h>

void CGeomBuilder::sphere(CMesh& mesh, int div1, int div2)
{

	for (int i = 0; i < div1; ++i)
	{

		float angle1 = M_PI / div1 * i;
		float angle2 = M_PI / div1 * (i + 1);

		float h1 = cos(angle1);
		float h2 = cos(angle2);

		float r1 = sin(angle1);
		float r2 = sin(angle2);

		for (int j = 0; j < div2; ++j)
		{

			float angle1 = 2.0 * M_PI / div2 * j;
			float angle2 = 2.0 * M_PI / div2 * (j + 1);

			// Do not add first triangle if this is the sphere's top.
			// This prevents a triangle with area 0 being created...
			// First triangle uses h1 twice.
			
			if (i != 0)
			{

				CPoly* poly1 = mesh.add(new CPoly);

				CEdge* edge1_1 = poly1->add(new CEdge);
				CEdge* edge1_2 = poly1->add(new CEdge);
				CEdge* edge1_3 = poly1->add(new CEdge);

				edge1_1->p[0] = sin(angle1) * r1;
				edge1_1->p[1] = cos(angle1) * r1;
				edge1_1->p[2] = h1;

				edge1_2->p[0] = sin(angle2) * r1;
				edge1_2->p[1] = cos(angle2) * r1;
				edge1_2->p[2] = h1;

				edge1_3->p[0] = sin(angle2) * r2;
				edge1_3->p[1] = cos(angle2) * r2;
				edge1_3->p[2] = h2;

				transform(poly1);

				poly1->init();
				
			}
			
			// Do not add second triangle if this is the sphere's bottom.
			// This prevents a triangle with area 0 being created...
			// Second triangle uses h2 twice.
			
			if (i != div1 - 1)
			{

				CPoly* poly2 = mesh.add(new CPoly);

				CEdge* edge2_1 = poly2->add(new CEdge);
				CEdge* edge2_2 = poly2->add(new CEdge);
				CEdge* edge2_3 = poly2->add(new CEdge);

				edge2_1->p[0] = sin(angle2) * r2;
				edge2_1->p[1] = cos(angle2) * r2;
				edge2_1->p[2] = h2;

				edge2_2->p[0] = sin(angle1) * r2;
				edge2_2->p[1] = cos(angle1) * r2;
				edge2_2->p[2] = h2;

				edge2_3->p[0] = sin(angle1) * r1;
				edge2_3->p[1] = cos(angle1) * r1;
				edge2_3->p[2] = h1;

				transform(poly2);

				poly2->init();
				
			}

		}

	}

}
