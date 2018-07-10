#include <gl/glu.h>
#include "../util/SOpenGL.h"
#include "marstd.cpp"

// === Matrix stuff ===

#define MATRIX_MODE_MARSTD 0
#define MATRIX_MODE_OPENGL 1

static int matrix_mode = MATRIX_MODE_MARSTD;

static void mat_identity();
static void mat_translate(float x, float y, float z);
static void mat_rotate(float x, float y, float z);
static void mat_scale(float x, float y, float z);
static void vertex(float x, float y, float z);

static void render();

static float t = 0.0;

int main(int argc, char* argv[])
{

    allegro_init();
    
    if (install_keyboard() < 0)
        return -1;

    if (SOpenGL::I().setGraphicsMode(640, 480, desktop_color_depth(), false) < 0)
        return -1;
        
	float p[3] =
	{
		0.0, 0.0, 0.0
	};
	
	float r[3] =
	{
		0.0, 0.0, 0.0
	};
	
    while (!key[KEY_ESC])
    {

		// Input.
		
		if (key[KEY_LEFT])
			p[0] -= 0.01;
		if (key[KEY_RIGHT])
			p[0] += 0.01;
		if (key[KEY_A])
			p[1] -= 0.01;
		if (key[KEY_Z])
			p[1] += 0.01;
		if (key[KEY_DOWN])
			p[2] -= 0.01;
		if (key[KEY_UP])
			p[2] += 0.01;
			
        // Do some OpenGL stuff.
        
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LESS);
//        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
	    gluPerspective(90, SCREEN_W / float(SCREEN_H), 0.1, 100);

		glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
		glEnable(GL_BLEND);

		glColor3ub(127, 127, 255);
		matrix_mode = MATRIX_MODE_OPENGL;
        render();

		glColor3ub(255, 127, 127);
		matrix_mode = MATRIX_MODE_MARSTD;
        render();
        
        // Make the colourmap visible.
    
        SOpenGL::I().flip();
        
        t += 0.01;
    
    }

    return 0;

} END_OF_MAIN();

// === Matrix stuff ===

static CMatrix4 matrix;

static void mat_identity()
{

	if (matrix_mode == MATRIX_MODE_MARSTD)
	{
		matrix.makeIdentity();
	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

}

static void mat_translate(float x, float y, float z)
{

	if (matrix_mode == MATRIX_MODE_MARSTD)
	{
		CMatrix4 temp;
		temp.makeTranslation(x, y, z);
		matrix *= temp;
	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(x, y, z);
	}
	
}

static void mat_rotate(float x, float y, float z)
{

	if (matrix_mode == MATRIX_MODE_MARSTD)
	{
		CMatrix4 temp;
		temp.makeRotation(x / 360.0 * 2.0 * M_PI, y / 360.0 * 2.0 * M_PI, z / 360.0 * 2.0 * M_PI);
		matrix *= temp;
	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glRotatef(x, 1.0, 0.0, 0.0);
		glRotatef(y, 0.0, 1.0, 0.0);
		glRotatef(z, 0.0, 0.0, 1.0);
	}

}

static void mat_scale(float x, float y, float z)
{

	if (matrix_mode == MATRIX_MODE_MARSTD)
	{
		CMatrix4 temp;
		temp.makeScaling(x, y, z);
		matrix *= temp;
	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glScalef(x, y, z);
	}
	
}

static void vertex(float x, float y, float z)
{

	if (matrix_mode == MATRIX_MODE_MARSTD)
	{
		matrix.mul4x3(x, y, z, &x, &y, &z);
		glVertex3f(x, y, z);
	}
	else
	{
		glVertex3f(x, y, z);
	}
	
}

static void render()
{

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		mat_identity();
    	mat_scale(1.0, -1.0, -1.0);
    	mat_translate(0.0, 0.0, 2.0);
    	mat_rotate(t * 10.0 * 1.0, t * 10.0 / 1.1111, t * 10.0 / 1.3333 * 1.0);
    	mat_scale(2.0, 0.5, 1.0);

        float v[3][4];

        for (int i = 0; i < 3; ++i)
        {

            v[i][0] = sin(i / 3.0 * 2.0 * M_PI);
            v[i][1] = cos(i / 3.0 * 2.0 * M_PI);
            v[i][2] = 0.0;
            v[i][3] = 1.0;

        }
        
        glBegin(GL_TRIANGLES);
        {

            for (int i = 0; i < 3; ++i)
            {

                vertex(v[i][0], v[i][1], v[i][2]);

            }

        }
        glEnd();
        
}
