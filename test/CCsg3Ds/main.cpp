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
static void renderMesh(CMesh& mesh);

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
	
	positionV[2] = 4.0;
	
	bool animate = true;

	while (!keyboard.wentDown(SDLK_ESCAPE))
	{

		framework.process();
		
		static float t = 0.0;

		if (keyboard.wentDown(SDLK_SPACE))
			animate = !animate;
		
		if (keyboard.isDown(SDLK_LEFT))
			positionV[0] -= 0.05;
		if (keyboard.isDown(SDLK_RIGHT))
			positionV[0] += 0.05;

		if (keyboard.isDown(SDLK_a))
			positionV[1] += 0.05;
		if (keyboard.isDown(SDLK_z))
			positionV[1] -= 0.05;

		if (keyboard.isDown(SDLK_DOWN))
			positionV[2] += 0.05;
		if (keyboard.isDown(SDLK_UP))
			positionV[2] -= 0.05;
		
		rotationV[1] -= mouse.dx / 10.0f;
		rotationV[0] -= mouse.dy / 10.0f;
		
		rotation[0] = t * 10.0;
		rotation[1] = t * 11.0;
		rotation[2] = t * 13.0;

		framework.beginDraw(0, 0, 0, 0);

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
//		glDepthMask(0);

		SOpenGL::I().setupStandardMatrices(0.0);
		
		gxRotatef(-rotationV[0], 1.0f, 0.0f, 0.0f);
		gxRotatef(-rotationV[1], 0.0f, 1.0f, 0.0f);
		gxRotatef(-rotationV[2], 0.0f, 0.0f, 1.0f);
		gxTranslatef(-positionV[0], -positionV[1], -positionV[2]);
		
		gxRotatef(t * 10.0, 1.0, 0.0, 0.0);
		gxRotatef(t * 10.0, 0.0, 1.0, 0.0);
		gxRotatef(t * 10.0, 0.0, 0.0, 1.0);

		glBlendFunc(GL_ONE, GL_ONE);
		glDisable(GL_BLEND);

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

#if 1
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
#else
		// Mesh 1 & 2 -> Mesh 3.		
//		CCsg3D::I().addition(mesh1, mesh2, mesh3);
		CCsg3D::I().subtraction(mesh1, mesh2, mesh3);
//		CCsg3D::I().intersection(mesh1, mesh2, mesh3);
		// Mesh 3 & 4 -> Mesh 5.
		if (keyboard.isDown(SDLK_1))
			CCsg3D::I().intersection(mesh3, mesh4, mesh5);
		else if (keyboard.isDown(SDLK_2))
			CCsg3D::I().subtraction(mesh3, mesh4, mesh5);
		else
			CCsg3D::I().addition(mesh3, mesh4, mesh5);
//		mesh3->move(mesh5);
#endif
		
		printf("PC: %d - OP: %f %f %f\n", mesh5->polyCount, objectPosition[0], objectPosition[1], objectPosition[2]);
		
		renderMesh(*mesh5);

		glDisable(GL_DEPTH_TEST);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		//renderMesh(*mesh5);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    		
		delete mesh1;
		delete mesh2;
		delete mesh3;
		delete mesh4;
		delete mesh5;

		framework.endDraw();

		if (animate)
		{
			objectPosition[0] += 0.001;
			objectPosition[1] += 0.001;
			objectPosition[2] += 0.001;
			objectRotation[0] += 0.003;
			objectRotation[1] += 0.002;
			objectRotation[2] += 0.001;
			
			t += 0.01;
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

static void renderMesh(CMesh& mesh)
{

    for (CPoly* poly = mesh.polyHead; poly; poly = poly->next)
    {

        gxColor3ub(
            int((poly->plane.normal[0] + 1.0) * 0.5 * 255),
            int((poly->plane.normal[1] + 1.0) * 0.5 * 255),
            int((poly->plane.normal[2] + 1.0) * 0.5 * 255));

//		gxColor3ub(63, 63, 63);
		
        gxBegin(GL_TRIANGLE_FAN);
        {
            for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
            {
                gxVertex3fv(edge->p);
            }
        }
        gxEnd();
        
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
