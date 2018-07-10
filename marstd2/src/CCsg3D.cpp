//////////////////////////////////////////////////////////////////////
// CCsg3D implementation.
//////////////////////////////////////////////////////////////////////

// To prevent splitting geometry:
//
// Remember polygon before split.
// If the poly gets split in a next recursion: do nothing.

// To fixup the resulting geometry:
//
// Per polygon: encapsulate the data member in our own data struct.
//   In the data struct: remember per polygon how many fractions are produced.
//   Remember numer of fractions front / back.
//   Reconstruct wholly if fractions front / back == total fractions produced.

CCsg3D::CCsg3D()
{

}

CCsg3D::~CCsg3D()
{

}

void CCsg3D::addition(CMesh* mesh1, CMesh* mesh2, CMesh* _out)
{

	MASSERT(mesh1);
	MASSERT(mesh2);
	MASSERT(_out);
	
	CMesh* out = new CMesh;
	CMesh* in = new CMesh;

	clip(mesh1, mesh2, out, in, 0);
	out->move(_out);
	in->clear();

	clip(mesh2, mesh1, out, in, 1);
	out->move(_out);
	in->clear();

	delete out;
	delete in;

}

void CCsg3D::subtraction(CMesh* mesh1, CMesh* mesh2, CMesh* _out)
{

	MASSERT(mesh1);
	MASSERT(mesh2);
	MASSERT(_out);
	
	CMesh* out = new CMesh;
	CMesh* in = new CMesh;

	clip(mesh1, mesh2, out, in, 0);
	out->move(_out);
	in->clear();

	clip(mesh2, mesh1, out, in, 1);
	// FIXME: For some reason the BSP compiler(?) doesn't like this reversal.. it crashes on multiple CSG actions..
	for (CPoly* poly = in->polyHead; poly; poly = poly->next)
		poly->reverseWinding();
	in->move(_out);
	out->clear();

	delete out;
	delete in;

}

void CCsg3D::intersection(CMesh* mesh1, CMesh* mesh2, CMesh* _out)
{

	MASSERT(mesh1);
	MASSERT(mesh2);
	MASSERT(_out);
	
	CMesh* out = new CMesh;
	CMesh* in = new CMesh;

	clip(mesh1, mesh2, out, in, 0);
	in->move(_out);
	out->clear();

	clip(mesh2, mesh1, out, in, 1);
	in->move(_out);
	out->clear();

	delete out;
	delete in;

}

void CCsg3D::exclusiveOr(CMesh* mesh1, CMesh* mesh2, CMesh* out)
{

	MASSERT(mesh1);
	MASSERT(mesh2);
	MASSERT(out);
	
}

void CCsg3D::clip(CMesh* mesh, CMesh* clipMesh, CMesh* out, CMesh* in, int polyClipPriority)
{

	MASSERT(mesh);
	MASSERT(clipMesh);
	MASSERT(out);
	MASSERT(in);
	
	if (mesh->polyCount == 0)
		return;
		
	if (clipMesh->polyCount == 0)
	{
		for (CPoly* poly = mesh->polyHead; poly; poly = poly->next)
			out->add(poly->copy());
		return;
	}
	
	CBsp* bsp = new CBsp;

	clipMesh->copy(bsp);

	bsp->split();

	clip(mesh, bsp, out, in, polyClipPriority);

}

void CCsg3D::clip(CMesh* mesh, CBsp* bsp, CMesh* out, CMesh* in, int polyClipPriority)
{

	MASSERT(mesh);
	MASSERT(bsp);
	MASSERT(out);
	MASSERT(in);

	if (!bsp->child[0] || !bsp->child[1])
	{

		// Child leaf!

		for (CPoly* poly = mesh->polyHead; poly; poly = poly->next)
		{
			if (poly->onFront(&bsp->plane) && poly->onBack(&bsp->plane))
			{

				// Coplanar, do a 2D CSG with the polygon in the BSP leaf.

				if (bsp->polyHead)
				{
     				if (!polyClipPriority)
					{
						CPoly* inTemp = new CPoly;
						poly->clip(bsp->polyHead, inTemp, out);
						if (inTemp->edgeCount >= 3)
							in->add(inTemp);
						else
							delete inTemp;
					}
					else
					{
						out->add(poly->copy());
					}
				}
				
			}
			else
			{

				// Possibly spanning.. clip polygon into front and back.

				CPoly* front = new CPoly;
				CPoly* back = new CPoly;
				
				poly->clip(&bsp->plane, front, back);
				
				if (front->edgeCount >= 3)
					out->add(front);
				else
					delete front;
					
				if (back->edgeCount >= 3)
					in->add(back);
				else
					delete back;
				
			}	
		}

	}
	else
	{

		// Clip all polygons and send them to front or back intermediate meshes..

		CMesh* tempMesh[2];

		tempMesh[0] = new CMesh;
		tempMesh[1] = new CMesh;

		for (CPoly* poly = mesh->polyHead; poly; poly = poly->next)
		{

			if (poly->onFront(&bsp->plane))
				tempMesh[0]->add(poly->copy());
			else if (poly->onBack(&bsp->plane))
				tempMesh[1]->add(poly->copy());
			else
			{
			#if 1
			CPoly* front = tempMesh[0]->add(new CPoly);
			CPoly* back = tempMesh[1]->add(new CPoly);
			poly->clip(&bsp->plane, front, back);
			#endif
			}

		}

		// Pass the intermediate meshes down to the children.

		for (int i = 0; i < 2; ++i)
		{
			if (tempMesh[i]->polyCount > 0)
				clip(tempMesh[i], bsp->child[i], out, in, polyClipPriority);
			delete tempMesh[i];
		}

	}

}

//////////////////////////////////////////////////////////////////////
