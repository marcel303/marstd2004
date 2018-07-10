//////////////////////////////////////////////////////////////////////
// CBsp implementation.
//////////////////////////////////////////////////////////////////////

inline void CBsp::move(CPoly* poly, CBsp* bsp)
{

	bsp->add(unlink(poly)); 	
	
}

inline int CBsp::test1(CBspSplitInfo* i1, CBspSplitInfo* i2)
{

	return (i1->sc < i2->sc || (i1->sc == i2->sc && i1->dc < i2->dc));

}	

inline int CBsp::test2(CBspSplitInfo* i1, CBspSplitInfo* i2)
{

	return (i1->dc < i2->dc || (i1->dc == i2->dc && i1->sc < i2->sc));

}	

inline int CBsp::test(CBspSplitInfo* i1, CBspSplitInfo* i2)
{

	// Try to reduce splits when the number of polygons is low.
	
	if (polyCount < 10)
		return test1(i1, i2);
	else
  		return test2(i1, i2);			
  		
}

//////////////////////////////////////////////////////////////////////
