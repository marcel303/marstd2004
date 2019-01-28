#include "marstd.h"
#include "../Util/SOpenGL.h"
#include "framework.h"

static CVector position;

static std::vector<CBsp*> bspArray;

static void render(CBsp& bsp);
static void render(CCompiledMesh& mesh, float opacity);

int main(int argc, char* argv[])
{

#if defined(CHIBI_RESOURCE_PATH)
	changeDirectory(CHIBI_RESOURCE_PATH);
#endif

	framework.enableDepthBuffer = true;
	
	if (!framework.init(640, 480))
		exit(-1);

#if 0
	allegro_message("\n
BSP tree class test app.
------------------------
This application will create a 'random' BSP tree anytime you press 'r'.
Use the mouse to rotate, cursor keys + A and Z to move.
This app doesn't use the z-buffer for rendering the object. :)

Currently the BSP compiler compiles just about any polygon soup without problems... small epsilon values will result in FPU rounding errors to occur though.
TODO: Calculate portals.
");
#endif

	CBsp bsp;
	CGeomBuilder::I().cube(bsp);
	CGeomBuilder::I().sphere(bsp, 10, 20);
	CGeomBuilder::I().cone(bsp, 10);
	bsp.split();

	CCompiledMesh pointMesh;
	CMesh temp;
	CGeomBuilder::I().matrix.push();
	CGeomBuilder::I().matrix.scale(0.1, 0.1, 0.1);
	CGeomBuilder::I().sphere(temp, 10, 10);
	CGeomBuilder::I().matrix.push();
	CGeomCompiler::I().compile(temp, pointMesh);
  	CVector point = CVector(0.0, 0.0, 0.0);

  	float time = 0.0;
  	
  	Camera3d camera;
  	
  	camera.position[2] = 2.f;
  	camera.yaw = 180.f;
  	
	while (!keyboard.wentDown(SDLK_ESCAPE))
	{

		framework.process();
		
		camera.tick(framework.timeStep, true);
		position = CVector(camera.position[0], camera.position[1], camera.position[2]);
		
		point[0] = sin(time / 1.111) * 0.75;
		point[1] = sin(time / 1.333) * 0.75;
		point[2] = sin(time / 1.555) * 0.75;

  		if (keyboard.isDown(SDLK_r))
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
		ry += mouse.dx;
		
		bspArray.clear();
		CSphere sphere;
		sphere.position = point;
		sphere.radius = 0.1;
		bsp.getHitLeafs(sphere, &bspArray);

		framework.beginDraw(0, 0, 0, 0);
		setBlend(BLEND_OPAQUE);
		
		SOpenGL::I().setupStandardMatrices(0.0, false);
		
		camera.pushViewMatrix();

		pushWireframe(keyboard.isDown(SDLK_w));
		{
			setDepthTest(true, DEPTH_ALWAYS, true);
			
			render(bsp);
		}
		popWireframe();
		
		gxPushMatrix();
		gxTranslatef(point[0], point[1], point[2]);

		setDepthTest(true, DEPTH_GEQUAL, false);

		pushWireframe(true);
		pushBlend(BLEND_ALPHA);
		render(pointMesh, .2f);
		popBlend();
		popWireframe();
		
		setDepthTest(true, DEPTH_LEQUAL, true);
		
		render(pointMesh, 1.f);
		
		gxPopMatrix();
		
		camera.popViewMatrix();

		framework.endDraw();

		time += framework.timeStep;

	}

	return 0;

}

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
		gxColor3ub(255, 255, 255);
	else
		gxColor3ub(0, 255, 0);
	
	for (CPoly* poly = bsp.polyHead; poly; poly = poly->next)
	{

		if (!inBspArray)
			setColorf(
				(poly->plane.normal[0] + .4f) / 1.4f,
				(poly->plane.normal[1] + .4f) / 1.4f,
				(poly->plane.normal[2] + .4f) / 1.4f);
		
		gxBegin(GX_TRIANGLE_FAN);
		{

			for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
			{

				gxVertex3fv(&edge->p[0]);

			}

		}
		gxEnd();

	}

}

static void render(CCompiledMesh& mesh, float opacity)
{

	for (int i = 0; i < (int)mesh.polygon.size(); ++i)
	{

		gxBegin(GX_TRIANGLE_FAN);
		{

			for (int j = 0; j < (int)mesh.polygon[i].vertex.size(); ++j)
			{
			
				setColorf(
					(mesh.vertex[mesh.polygon[i].vertex[j]].plane.normal[0] + .4f) / 1.4f,
					(mesh.vertex[mesh.polygon[i].vertex[j]].plane.normal[1] + .4f) / 1.4f,
					(mesh.vertex[mesh.polygon[i].vertex[j]].plane.normal[2] + .4f) / 1.4f,
					opacity);
				gxVertex3fv(mesh.vertex[mesh.polygon[i].vertex[j]].p);

			}

		}
		gxEnd();

	}

}
