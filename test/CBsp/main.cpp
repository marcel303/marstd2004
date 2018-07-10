#include <allegro.h>
#include "marstd.cpp"
#include "../util/SOpenGL.h"

static CVector position;

static std::vector<CBsp*> bspArray;

static void render(CBsp& bsp);
static void render(CCompiledMesh& mesh);

int main(int argc, char* argv[])
{

	allegro_init();

	allegro_message("\n
BSP tree class test app.
------------------------
This application will create a 'random' BSP tree anytime you press 'r'.
Use the mouse to rotate, cursor keys + A and Z to move.
This app doesn't use the z-buffer for rendering the object. :)

Currently the BSP compiler compiles just about any polygon soup without problems... small epsilon values will result in FPU rounding errors to occur though.
TODO: Calculate portals.
");
	if (install_keyboard() < 0)
		exit(-1);

	if (install_mouse() < 0)
		exit(-1);

	CBsp bsp;
	CGeomBuilder::I().cube(bsp);
	CGeomBuilder::I().sphere(bsp, 10, 20);
	CGeomBuilder::I().cone(bsp, 10);
	bsp.split();

	if (SOpenGL::I().setGraphicsMode(640, 480, desktop_color_depth(), false) < 0)
		exit(-1);

	CCompiledMesh pointMesh;
	CMesh temp;
	CGeomBuilder::I().matrix.push();
	CGeomBuilder::I().matrix.scale(0.1, 0.1, 0.1);
	CGeomBuilder::I().sphere(temp, 10, 10);
	CGeomBuilder::I().matrix.push();
	CGeomCompiler::I().compile(temp, pointMesh);
  	CVector point = CVector(0.0, 0.0, 0.0);

  	float time = 0.0;
  	
  	position[2] = 2.0;
  	
	while (!key[KEY_ESC])
	{

		point[0] = sin(time / 10.111) * 0.75;
		point[1] = sin(time / 10.333) * 0.75;
		point[2] = sin(time / 10.555) * 0.75;
		
		if (key[KEY_LEFT])
			position[0] -= 0.01;
		if (key[KEY_RIGHT])
			position[0] += 0.01;
		if (key[KEY_A])
			position[1] -= 0.01;
		if (key[KEY_Z])
			position[1] += 0.01;
		if (key[KEY_UP])
			position[2] -= 0.01;
		if (key[KEY_DOWN])
			position[2] += 0.01;

  		if (key[KEY_R])
  		{

			bsp.clear();
  			float scale = (rand()&1023) / 1023.0 + 0.5;
			CGeomBuilder::I().matrix.identity();
			CGeomBuilder::I().matrix.scale(scale, scale, scale);
			CGeomBuilder::I().cube(bsp);
  			scale = (rand()&1023) / 1023.0 + 0.5;
			CGeomBuilder::I().matrix.identity();
			CGeomBuilder::I().matrix.scale(scale, scale, scale);
			CGeomBuilder::I().sphere(bsp, 10, 10);
  			scale = (rand()&1023) / 1023.0 + 0.5;
			CGeomBuilder::I().matrix.identity();
			CGeomBuilder::I().matrix.scale(scale, scale, scale);
			CGeomBuilder::I().cone(bsp, 10);
			bsp.split();

		}
		
		static float ry;
        int mx, my;
		get_mouse_mickeys(&mx, &my);
		ry += mx;
		
		bspArray.clear();
		CSphere sphere;
		sphere.position = point;
		sphere.radius = 0.1;
		bsp.getHitLeafs(sphere, &bspArray);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SOpenGL::I().setupStandardMatrices(0.0);
		glRotatef(ry, 0.0, 1.0, 0.0);
		glTranslatef(-position[0], -position[1], -position[2]);

		if (key[KEY_W])
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   		glDisable(GL_DEPTH_TEST);
   		
		render(bsp);
		
		glPushMatrix();
		glTranslatef(point[0], point[1], point[2]);

		glDepthFunc(GL_GEQUAL);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(0);

		glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
		glEnable(GL_BLEND);
		
		render(pointMesh);
		
		glDisable(GL_BLEND);
		
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(1);
		
		render(pointMesh);
		
		glPopMatrix();

		SOpenGL::I().flip();

		time += 1.0 / 10.0;

	}

	return 0;

} END_OF_MAIN();

static void render(CBsp& bsp)
{

	float d = bsp.plane * ::position;
	
	if (d >= 0.0)
	{
		if (bsp.child[1])
			render(*bsp.child[1]);
		if (bsp.child[0])
			render(*bsp.child[0]);
	}
	else
	{
		if (bsp.child[0])
			render(*bsp.child[0]);
		if (bsp.child[1])
			render(*bsp.child[1]);
	}

	bool inBspArray = false;
	for (int i = 0; i < (int)bspArray.size() && !inBspArray; ++i)
	{
		if (bspArray[i] == &bsp)
			inBspArray = true;
	}
	
	if (inBspArray)
		glColor3ub(255, 255, 255);
	else
		glColor3ub(0, 255, 0);
	
	for (CPoly* poly = bsp.polyHead; poly; poly = poly->next)
	{

		if (!inBspArray)
			glColor3fv(&poly->plane.normal[0]);
		
		glBegin(GL_POLYGON);
		{

			for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
			{

				glVertex3fv(&edge->p[0]);

			}

		}
		glEnd();

	}

}

static void render(CCompiledMesh& mesh)
{

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	for (int i = 0; i < (int)mesh.polygon.size(); ++i)
	{

		glBegin(GL_POLYGON);
		{

			for (int j = 0; j < (int)mesh.polygon[i].vertex.size(); ++j)
			{

				glColor3fv(mesh.vertex[mesh.polygon[i].vertex[j]].plane.normal);
				glVertex3fv(mesh.vertex[mesh.polygon[i].vertex[j]].p);

			}

		}
		glEnd();

	}

}
