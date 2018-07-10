#ifndef __SOpenGL_h__
#define __SOpenGL_h__

#include "framework.h"

class SOpenGL
{
    
    private:
    
    SOpenGL();
    
    public:
    
    ~SOpenGL();
    
    public:
    
    static SOpenGL& I()
    {
        static SOpenGL openGL;
        return openGL;
    }

    public:

	void setupStandardMatrices(float z = -2.0, bool invertZ = true);
    
};

SOpenGL::SOpenGL()
{
	
}

SOpenGL::~SOpenGL()
{

}

void SOpenGL::setupStandardMatrices(float z, bool invertZ)
{

	projectPerspective3d(90.f, .1f, 100.f);
	if (invertZ)
		gxScalef(1.f, 1.f, -1.f);
	gxTranslatef(0.f, 0.f, z);

}

#endif
