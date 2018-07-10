//////////////////////////////////////////////////////////////////////
// CIsosurface text.
//////////////////////////////////////////////////////////////////////

#include <alleggl.h>
#include <gl/glu.h>
#include <math.h>
#include "marstd.cpp"
#include "../util/SOpenGL.h"
#include "../util/SOpenGLTextureManager.h"

//////////////////////////////////////////////////////////////////////
// Could also use isosurfaces to do csg add, substract, xor, etc like in some raytracers.
// simple implementation requires a high resolution though.
//////////////////////////////////////////////////////////////////////

static void draw_triangles(int num, CIsosurfaceVertex* v);

int main(int argc, char* argv[])
{

//--------------------------------------------------------------------
// Initialize system.

	allegro_init();
	
	allegro_message("MarSTD\n------\nCIsosurface & CIsosurfaceMetaball classes example.\nPress L to render as wireframe.\nPress I to invert metaball energy values.\nPress mouse button 1 & 2 to change treshold.\n------\nLogic execution rate is currently equal to the number of frames per second it renders.\nIf the example appears to execute too fast, enable vsync in your display properties for OpenGL applications.\n------\nMarcel Smit, 2003.");

	if (install_timer() < 0 || install_keyboard() < 0 || install_mouse() < 0)
	{
		allegro_message("Error: unable to install system drivers.");
		return -1;
	}		
	
//--------------------------------------------------------------------
// Open an OpenGL window.

	if (SOpenGL::I().setGraphicsMode(800, 600, desktop_color_depth(), false) < 0)
	{
		allegro_message("Error: unable to set OpenGL graphics mode.");
		return -1;
	}
	
	// Field with medium resolution.
 	
	CIsosurface field;		// This is our CIsosurface object.
//	field.setSize(20, 20, 20);
	field.setSize(30, 30, 30);
//	field.setSize(50, 50, 50);

	// Four metaballs.
	
	const int balls = 4;
	CIsosurfaceMetaball metaball[balls];
	
	// Textures.
	
	GLuint envmap = SOpenGLTextureManager::I().addTextureFromFile("data/envmap.bmp");
	GLuint texmap = SOpenGLTextureManager::I().addTextureFromFile("data/texmap.bmp");
 	
//--------------------------------------------------------------------
// Main loop.
 	
	while (!key[KEY_ESC])
	{
	
		static float t = 0.0;
		static float ry = 0.0;
	
//--------------------------------------------------------------------
// Input.
	
		int mx, my;
		get_mouse_mickeys(&mx, &my);
		ry += mx;
		
		if (mouse_b & 1)
			field.treshold += 0.01;
		if (mouse_b & 2)
			field.treshold -= 0.01;			
   	
//--------------------------------------------------------------------
// Update / logic.
	
		// Animate metaballs.
		
		for (int i = 0; i < balls; ++i)
		{
		
			metaball[i].p[0] = sin(t / (111.111 + i * 11.1)) * 0.6;
			metaball[i].p[1] = cos(t / (133.333 + i * 13.3)) * 0.6;
			metaball[i].p[2] = sin(t / (266.666 + i * 26.6)) * 0.6;
			metaball[i].a = +0.05;				
		
		}

		// Calculate energy values using metaballs.
		
		field.calculate(metaball, balls);
		
		#if 1
		
		// Add 2 "lines".
		
		float s1 = (cos(t / 1111.111 * 1.3) + 1.5) / 2.5;
		float s2 = (cos(t / 1333.333 * 1.3) + 1.5) / 2.5;
		for (int i = 0; i < field.sx; ++i)
			for (int j = 0; j < field.sy; ++j)
				for (int k = 0; k < field.sz; ++k)
				{
				
					float dx, dy, dz, d, e = 0.0;
					
                    dx = field.v[i][j][k].p[0];
					dy = 0.0;
					dz = field.v[i][j][k].p[2] * s1;					
					d = dx * dx + dy * dy + dz * dz;
					
					e += 0.02 * field.treshold / (d + 0.001);
					
                    dx = 0.0;
					dy = field.v[i][j][k].p[1] * s2;
					dz = field.v[i][j][k].p[2];					
					d = dx * dx + dy * dy + dz * dz;
					
					e += 0.02 * field.treshold / (d + 0.001);
					
					field.v[i][j][k].e += e;
				
				}
				
		#endif				
		
		// We will be using the normal vectors, so calculate them.

		field.calculateNormals();

		// Invert?
		
		if (key[KEY_I])
		{
			for (int i = 0; i < field.sx; ++i)
				for (int j = 0; j < field.sy; ++j)
					for (int k = 0; k < field.sz; ++k)
						field.v[i][j][k].e = -field.v[i][j][k].e;
		}		

		t += 1.0;
		ry += 0.3;
		
//--------------------------------------------------------------------
// Render.
		
		// Setup matrices.

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		gluPerspective(90.0, SCREEN_W / (float)SCREEN_H, 0.001, 100.0);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		// Prepare buffers.
		
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);		
		
		// Setup and enable lighting.

		GLfloat l_direction[4][4] =
		{
			{ +1.0, +1.0, 0.0, 0.0 },
			{ -1.0, +1.0, 0.0, 0.0 },
			{ -1.0, -1.0, 0.0, 0.0 },
			{ +1.0, -1.0, 0.0, 0.0 }
		};
			
		GLfloat l_diffuse[4][4] =
		{
			{ 1.5, 0.5, 0.5, 1.0 },
			{ 1.5, 0.5, 1.5, 1.0 },
			{ 0.5, 0.5, 1.5, 1.0 },
			{ 2.0, 2.0, 2.0, 1.0 }
		};
			
		for (int i  =0; i < 4; ++i)
		{
   			if (0)
				for (int j = 0; j < 3; ++j)
					if (l_diffuse[i][j] == 0.5)
						l_diffuse[i][j] = 1.0;
			glLightfv(GL_LIGHT1 + i, GL_POSITION, l_direction[i]);
			glLightfv(GL_LIGHT1 + i, GL_DIFFUSE, l_diffuse[i]);
			glEnable(GL_LIGHT1 + i);
		}
  			
		glEnable(GL_LIGHTING);

		// Camera transformation.
		
		glTranslatef(0.0, 0.0, -2.0);
		glTranslatef(0.0, (sin(t * 0.003) - 0.5) / 3.0, 0.0);
		glRotatef(ry / 1.111, 0.0, 1.0, 0.0);

		// Enable sphere mapping. This will give a nice reflective effect.
		
		glBindTexture(GL_TEXTURE_2D, envmap);
		glEnable(GL_TEXTURE_2D);
		
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);		
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);

		if (0)
		{
			// Cannot enable culling while mirroring too, because normals aren't mirrored as well..		
			glCullFace(GL_FRONT);		
			glEnable(GL_CULL_FACE);
		}
		
		CIsosurfaceVertex v[600];		// Vertex buffer.
		field.output(200, v, draw_triangles);	// Render triangles using specified callback function.
		
		// Draw floor.
	
		glBindTexture(GL_TEXTURE_2D, texmap);
  	
		glDepthMask(0);		// We disable writing to the depth buffer. When drawing multiple unsorted transparent surfaces, you will need to do this or suffer the consequences.

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);	
		glColor4f(1.0, 1.0, 1.0, 0.5);
	
		glBegin(GL_QUADS);
		{
	
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-2.0, -1.0, -2.0);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(+2.0, -1.0, -2.0);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(+2.0, -1.0, +2.0);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-2.0, -1.0, +2.0);   
			
		} glEnd();
	
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);		
		glEnable(GL_LIGHTING);
	
		glDisable(GL_BLEND);
	
		glDepthMask(1);
 		
//--------------------------------------------------------------------
// Make back buffer visible.
		
		allegro_gl_flip();
	
	}

//--------------------------------------------------------------------
// Shutdown system.

	remove_allegro_gl();
	allegro_exit();
	
	return 0;

} END_OF_MAIN();

//--------------------------------------------------------------------
// Triangles drawer.

static void draw_triangles(int num, CIsosurfaceVertex* v)
{

	// Use line mode?
	
	if (key[KEY_L])
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		
		
	// Use vertex pointers to draw faster.
	
	glVertexPointer(3, GL_FLOAT, sizeof(CIsosurfaceVertex), v->p);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(CIsosurfaceVertex), v->n);
	glEnableClientState(GL_NORMAL_ARRAY);	
	
	// Draw vertex arrays.
	
	glDrawArrays(GL_TRIANGLES, 0, num * 3);
	
	// Mirror y coordinate.
	
	for (int i = 0; i < num * 3; ++i)
	{
	
		v[i].p[1] = -v[i].p[1] - 2.0;
	
	}
	
	// Draw vertex arrays a second time.
	
	glDrawArrays(GL_TRIANGLES, 0, num * 3);

	if (0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, num * 3);
	}		
	
}
