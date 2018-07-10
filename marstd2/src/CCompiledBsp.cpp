//////////////////////////////////////////////////////////////////////
// CCompiledBsp implementation.
//////////////////////////////////////////////////////////////////////

CCompiledBsp::CCompiledBsp()
{

	child[0] = child[1] = 0;

	root = 0;

}

CCompiledBsp::~CCompiledBsp()
{

}

void CCompiledMesh::compile(CBsp* bsp)
{

	this->root = this;

	std::vector<CBsp*> bspArray;

	getLeafNodes(bsp, bspArray);

	CMesh temp;

	CGeomCompiler::I().compile(temp, *this);

	replicateHierarchy(bsp)

	std::vector<CCompiledBsp*> cbspArray;

	getLeafNodes(cbspArray);

	for (int i = 0; i < cbspArray.size(); ++i)
	{
		CGeomCompiler::I().compile(*bspArray[i], *this, *cbspArray[i]);
	}

}

void CCompiledBsp::getLeafNodes(CBsp* bsp, std::vector<CBsp*>& bspArray)
{

	for (int i = 0; i < 2; ++i)
		if (bsp->child[i])
			bsp->child[i].getLeafNodes(bsp, bspArray);

	if (!bsp->child[0] && !bsp->child[1])
		bspArray.push_back(bsp);

}

void getLeafNodes(std::vector<CCompiledBsp*>& cbspArray)
{

	for (int i = 0; i < 2; ++i)
		if (child[i])
			child[i].getLeafNodes(bsp, bspArray);

	if (!child[0] && !child[1])
		bspArray.push_back(this);

}

void CCompiledBsp::replicateHierarchy(CBsp* bsp)
{

	for (int i = 0; i < 2; ++i)
	{
		if (bsp->child[i])
		{
			child[i] = new CCompiledBsp;
			child[i]->plane = bsp->plane;
			child[i]->root = this->root;
			child[i]->replicateHierarchy(bsp->child[i]);
		}
	}

}

//////////////////////////////////////////////////////////////////////
