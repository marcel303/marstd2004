#include "../Util/SOpenGL.h"
#include "marstd.h"
#include "framework.h"

static CVector rotationV;
static CVector positionV;
static CVector rotation;
static CVector objectPosition;
static CVector objectRotation;

static void renderBsp(CBsp* bsp);
static void renderPoly(CPoly* poly);
static void renderMesh(CMesh& mesh, float fillOpacity, bool line);

int main(int argc, char* argv[])
{

	framework.enableDepthBuffer = true;
	
	if (!framework.init(0, nullptr, 1024, 768))
		exit(-1);

	const char* message =
		"press ARROW KEYS + A/Z to move around\n" \
		"press 1-4 to change CSG operation\n" \
		"press Q/W/E to change CSG shape\n" \
		"press SPACE to toggle CSG shape animation";
	
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Instructions.", message, nullptr);
	
	Camera3d camera;
	camera.yaw = 180.f;
	camera.position[2] = 4.f;
	
	bool animate = true;

	float t = 0.f;
	
	while (!keyboard.wentDown(SDLK_ESCAPE))
	{

		framework.process();
		
		const float dt = framework.timeStep;

		if (keyboard.wentDown(SDLK_SPACE))
			animate = !animate;
	
		camera.tick(framework.timeStep, true);
		
		positionV = CVector(camera.position[0], camera.position[1], camera.position[2]);
	
		rotation[0] = t * 10.0;
		rotation[1] = t * 11.0;
		rotation[2] = t * 13.0;

		framework.beginDraw(0, 0, 0, 0);

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);

		SOpenGL::I().setupStandardMatrices(0.0, false);
		
		camera.pushViewMatrix();
		
		gxRotatef(t * 10.0, 1.0, 0.0, 0.0);
		gxRotatef(t * 10.0, 0.0, 1.0, 0.0);
		gxRotatef(t * 10.0, 0.0, 0.0, 1.0);

		CMesh* mesh1 = new CMesh;
		CMesh* mesh2 = new CMesh;
		CMesh* mesh3 = new CMesh;
		CMesh* mesh4 = new CMesh;		
		CMesh* mesh5 = new CMesh;				
		
		// Mesh 1.
		if (keyboard.isDown(SDLK_w))
			CGeomBuilder::I().cilinder(*mesh1, 10);
		if (keyboard.isDown(SDLK_e))
			CGeomBuilder::I().donut(*mesh1, 5, 5, 0.75, 0.5);
		else
			CGeomBuilder::I().cube(*mesh1);
		
		// Mesh 2.
		CGeomBuilder::I().matrix.push();
		CGeomBuilder::I().matrix.translate(objectPosition);
		CGeomBuilder::I().matrix.rotate(objectPosition);
		if (keyboard.isDown(SDLK_q))
			CGeomBuilder::I().cilinder(*mesh2, 10);
		else
			CGeomBuilder::I().donut(*mesh2, 5, 5, 0.75, 0.5);
//		CGeomBuilder::I().cube(*mesh2);
		CGeomBuilder::I().matrix.pop();
		
		// Mesh 4.
		CGeomBuilder::I().matrix.push();
		CGeomBuilder::I().matrix.rotate(objectPosition * 2.0);		
		CGeomBuilder::I().matrix.scale(0.2, 0.2, 0.2);				
		CGeomBuilder::I().cube(*mesh4);
//		CGeomBuilder::I().donut(*mesh4, 5, 5, 0.75, 0.5);
//		CGeomBuilder::I().cilinder(*mesh4, 10);
		CGeomBuilder::I().matrix.pop();
		
		// Mesh 1 & 2 -> Mesh 3.
		if (keyboard.isDown(SDLK_1))
			CCsg3D::I().addition(mesh1, mesh2, mesh3);
		else if (keyboard.isDown(SDLK_2))
			CCsg3D::I().intersection(mesh1, mesh2, mesh3);
		else
			CCsg3D::I().subtraction(mesh1, mesh2, mesh3);
		
		// Mesh 3 & 4 -> Mesh 5.
		if (keyboard.isDown(SDLK_3))
			CCsg3D::I().subtraction(mesh3, mesh4, mesh5);
		if (keyboard.isDown(SDLK_4))
			CCsg3D::I().intersection(mesh3, mesh4, mesh5);
		else
			CCsg3D::I().addition(mesh3, mesh4, mesh5);
//		mesh3->move(*mesh5);

		printf("PC: %d - OP: %f %f %f\n", mesh5->polyCount, objectPosition[0], objectPosition[1], objectPosition[2]);
		
		pushBlend(BLEND_OPAQUE);
		renderMesh(*mesh5, 1.f, false);
		popBlend();
		
		glDepthMask(GL_FALSE);
		pushBlend(BLEND_ALPHA);
		renderMesh(*mesh1, .1f, true);
		renderMesh(*mesh2, .1f, true);
		renderMesh(*mesh3, .1f, true);
		renderMesh(*mesh4, .1f, true);
		popBlend();
		glDepthMask(GL_TRUE);
		
		glDisable(GL_DEPTH_TEST);
    		
		delete mesh1;
		delete mesh2;
		delete mesh3;
		delete mesh4;
		delete mesh5;
		
		camera.popViewMatrix();

		framework.endDraw();

		if (animate)
		{
			objectPosition[0] = sinf(t / 2.f / 2.345f) * 1.2f;
			objectPosition[1] = sinf(t / 2.f / 3.456f) * 1.3f;
			objectPosition[2] = sinf(t / 2.f / 4.567f) * 1.4f;
			objectRotation[0] += 0.3 * dt / 2.f;
			objectRotation[1] += 0.2 * dt / 2.f;
			objectRotation[2] += 0.1 * dt / 2.f;
			
			t += dt;
		}

	}

	return 0;

}

static void doRenderBsp(CBsp* bsp)
{

	if (!bsp->child[0] || !bsp->child[1])
	{
		for (CPoly* poly = bsp->polyHead; poly; poly = poly->next)
			renderPoly(poly);
	}
	else
	{
		float d = bsp->plane * positionV;
		if (d >= 0.0)
		{
			doRenderBsp(bsp->child[1]);
			doRenderBsp(bsp->child[0]);
		}
		else
		{
			doRenderBsp(bsp->child[0]);
			doRenderBsp(bsp->child[1]);
		}
	}

}

static void renderBsp(CBsp* bsp)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	doRenderBsp(bsp);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	doRenderBsp(bsp);

}

static void renderPoly(CPoly* poly)
{

	float tmp[3];
	for (int i = 0; i < 3; ++i)
		tmp[i] = (poly->plane.normal[i] + 1.0) * 0.5;
	setColorf(tmp[0], tmp[1], tmp[2]);

	gxBegin(GL_TRIANGLE_FAN);
	{

		for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
		{

			gxVertex3f(edge->p[0], edge->p[1], edge->p[2]);

		}

	}
	gxEnd();

}

static void renderMesh(CMesh& mesh, float fillOpacity, bool line)
{

    for (CPoly* poly = mesh.polyHead; poly; poly = poly->next)
    {

		if (fillOpacity > 0.f)
		{
			gxColor4f(
				(poly->plane.normal[0] + 1.0) * 0.5,
				(poly->plane.normal[1] + 1.0) * 0.5,
				(poly->plane.normal[2] + 1.0) * 0.5,
				fillOpacity);

			//gxColor3ub(63, 63, 63);
			
			gxBegin(GL_TRIANGLE_FAN);
			{
				for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
				{
					gxVertex3fv(edge->p);
				}
			}
			gxEnd();
		}
		
		if (line)
		{
			gxColor3ub(63, 63, 63);
			
			gxBegin(GL_LINES);
			{
				for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
				{
					gxVertex3fv(edge->p);
					gxVertex3fv(edge->edge2->p);
				}
			}
			gxEnd();
		}
        
        gxColor3ub(255, 255, 255);
        
        gxBegin(GL_LINES);
        {
            for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
            {
            	CVector p1 = edge->p;
            	CVector p2 = edge->p + poly->plane.normal * 0.1;
                gxVertex3fv(p1);
                gxVertex3fv(p2);
            }
        }
        gxEnd();
        
    }
        
}
