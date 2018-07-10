#ifndef __SOpenGL_h__
#define __SOpenGL_h__

#include <alleggl.h>
#include <gl/glu.h>
#include <vector>

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
    
    std::vector<const void (*)()> flipCallbackArray;
    
    public:
    
	void registerFlipCallback(const void (*flipCallback)());
	int setGraphicsMode(int w, int h, int bpp, bool fullscreen);
    void flip();

    public:

	void setupStandardMatrices(float z = -2.0);
    
};

SOpenGL::SOpenGL()
{

	install_allegro_gl();

	// Make sure OpenGL librairy gets loaded by setting a graphics mode.

	allegro_gl_set(AGL_COLOR_DEPTH, desktop_color_depth());
	allegro_gl_set(AGL_DOUBLEBUFFER, 1);
	allegro_gl_set(AGL_REQUIRE, AGL_COLOR_DEPTH | AGL_DOUBLEBUFFER);

	allegro_gl_set(AGL_Z_DEPTH, 24);
 	allegro_gl_set(AGL_SUGGEST, AGL_Z_DEPTH);

	set_gfx_mode(GFX_OPENGL_WINDOWED, 320, 240, 0, 0);
	
}

SOpenGL::~SOpenGL()
{

	remove_allegro_gl();

}

void SOpenGL::registerFlipCallback(const void (*flipCallback)())
{

    flipCallbackArray.push_back(flipCallback);

}

int SOpenGL::setGraphicsMode(int w, int h, int bpp, bool fullscreen)
{

	allegro_gl_set(AGL_COLOR_DEPTH, bpp);
	allegro_gl_set(AGL_DOUBLEBUFFER, 1);
	allegro_gl_set(AGL_REQUIRE, AGL_COLOR_DEPTH | AGL_DOUBLEBUFFER);

	allegro_gl_set(AGL_Z_DEPTH, 24);
 	allegro_gl_set(AGL_SUGGEST, AGL_Z_DEPTH);

	if (set_gfx_mode(fullscreen ? GFX_OPENGL_FULLSCREEN : GFX_OPENGL_WINDOWED, w, h, 0, 0) < 0)
		return -1;

	return 1;

}

void SOpenGL::flip()
{

	for (std::vector<const void (*)()>::iterator ptr = flipCallbackArray.begin(); ptr != flipCallbackArray.end(); ++ptr)
		(*ptr)();

	allegro_gl_flip();

}

void SOpenGL::setupStandardMatrices(float z)
{

        glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();

    	glScalef(1.0, -1.0, 1.0);

	gluPerspective(90, SCREEN_W / float(SCREEN_H), 0.1, 100);

    	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    	glTranslatef(0.0, 0.0, z);

}

#endif
