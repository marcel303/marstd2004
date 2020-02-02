//////////////////////////////////////////////////////////////////////
// CIsosurface text.
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "marstd.h"
#include "../Util/SOpenGL.h"
#include "framework.h"
#include <SDL2/SDL_messagebox.h>

//////////////////////////////////////////////////////////////////////
// Could also use isosurfaces to do csg add, substract, xor, etc like in some raytracers.
// simple implementation requires a high resolution though.
//////////////////////////////////////////////////////////////////////

static void draw_triangles(int num, CIsosurfaceVertex* v);

static GxTextureId envmap = 0;
static GxTextureId texmap = 0;

int main(int argc, char* argv[])
{

#if defined(CHIBI_RESOURCE_PATH)
	changeDirectory(CHIBI_RESOURCE_PATH);
#endif

//--------------------------------------------------------------------
// Initialize system.

	SDL_ShowSimpleMessageBox(0, "Instruction", "MarSTD\n------\nCIsosurface & CIsosurfaceMetaball classes example.\nPress L to render as wireframe.\nPress I to invert metaball energy values.\nPress mouse button 1 & 2 to change treshold.\n------\nLogic execution rate is currently equal to the number of frames per second it renders.\nIf the example appears to execute too fast, enable vsync in your display properties for OpenGL applications.\n------\nMarcel Smit, 2003.", nullptr);
	
//--------------------------------------------------------------------
// Open an OpenGL window.

	framework.enableDepthBuffer = true;
	
	framework.enableRealTimeEditing = true;
	
	if (!framework.init(800, 600))
	{
		//allegro_message("Error: unable to set OpenGL graphics mode.");
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
	
	envmap = getTexture("data/envmap.bmp");
	texmap = getTexture("data/texmap.bmp");
 	
//--------------------------------------------------------------------
// Main loop.
 	
	while (!keyboard.wentDown(SDLK_ESCAPE))
	{
	
		framework.process();
		
		static float t = 0.0;
		static float ry = 0.0;
	
//--------------------------------------------------------------------
// Input.
	
		ry += mouse.dx;
		
		if (mouse.isDown(BUTTON_LEFT))
			field.treshold += 0.01;
		if (mouse.isDown(BUTTON_RIGHT))
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
		
		if (keyboard.isDown(SDLK_i))
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

		framework.beginDraw(0, 0, 0, 0);
		
		// Setup matrices.
		
		projectPerspective3d(90.f, .001f, 100.f);
		
		setDepthTest(true, DEPTH_LESS);
		setBlend(BLEND_OPAQUE);
		
	#if 0 // todo : reimplement lighting
		// Setup and enable lighting.

		float l_direction[4][4] =
		{
			{ +1.0, +1.0, 0.0, 0.0 },
			{ -1.0, +1.0, 0.0, 0.0 },
			{ -1.0, -1.0, 0.0, 0.0 },
			{ +1.0, -1.0, 0.0, 0.0 }
		};
			
		float l_diffuse[4][4] =
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
	#endif

		// Camera transformation.
		
		gxTranslatef(0.0, 0.0, 2.0);
		gxTranslatef(0.0, (sin(t * 0.003) - 0.5) / 3.0, 0.0);
		gxRotatef(ry / 1.111, 0.0, 1.0, 0.0);
	
	#if 0
		if (0)
		{
			// Cannot enable culling while mirroring too, because normals aren't mirrored as well..		
			glCullFace(GL_FRONT);		
			glEnable(GL_CULL_FACE);
		}
	#endif
		
		CIsosurfaceVertex v[600];		// Vertex buffer.
		field.output(200, v, draw_triangles);	// Render triangles using specified callback function.
		
		// Draw floor.
	
		gxSetTexture(texmap);
  	
		pushDepthWrite(false);		// We disable writing to the depth buffer. When drawing multiple unsorted transparent surfaces, you will need to do this or suffer the consequences.

	// todo
		//glDisable(GL_LIGHTING);

		pushBlend(BLEND_ALPHA);
		gxColor4f(1.0, 1.0, 1.0, 0.5);
	
		gxBegin(GX_QUADS);
		{
	
			gxTexCoord2f(0.0, 0.0);
			gxVertex3f(-2.0, -1.0, -2.0);
			gxTexCoord2f(1.0, 0.0);
			gxVertex3f(+2.0, -1.0, -2.0);
			gxTexCoord2f(1.0, 1.0);
			gxVertex3f(+2.0, -1.0, +2.0);
			gxTexCoord2f(0.0, 1.0);
			gxVertex3f(-2.0, -1.0, +2.0);
			
		}
		gxEnd();
	
	// todo
		//glEnable(GL_LIGHTING);
	
		popBlend();
		
		popDepthWrite();
		
		gxSetTexture(0);
 		
//--------------------------------------------------------------------
// Make back buffer visible.
		
		framework.endDraw();
	
	}

//--------------------------------------------------------------------
// Shutdown system.

	framework.shutdown();
	
	return 0;

}

//--------------------------------------------------------------------
// Triangles drawer.

static void draw_triangles(int num, CIsosurfaceVertex* v)
{

	// Use line mode?
	
	pushWireframe(keyboard.isDown(SDLK_l));
	
	// Enable sphere mapping. This will give a nice reflective effect.
	
	Shader shader("data/envmap");
	setShader(shader);
	shader.setTexture("envmap", 0, envmap, true);
	
	// Draw vertex arrays.
	
	gxBegin(GX_TRIANGLES);
	{
		for (int i = 0; i < num * 3; ++i)
		{
			gxNormal3fv(v[i].n);
			gxVertex3fv(v[i].p);
		}
	}
	gxEnd();
	
	// Mirror y coordinate.
	
	for (int i = 0; i < num * 3; ++i)
	{
	
		v[i].p[1] = -v[i].p[1] - 2.0;
	
	}
	
	// Draw vertex arrays a second time.
	
	gxBegin(GX_TRIANGLES);
	{
		for (int i = 0; i < num * 3; ++i)
		{
			gxNormal3fv(v[i].n);
			gxVertex3fv(v[i].p);
		}
	}
	gxEnd();

#if 0

	if (0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, num * 3);
	}
	
#endif

	clearShader();

	popWireframe();
	
}
