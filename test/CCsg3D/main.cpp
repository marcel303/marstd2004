#include "marstd.cpp"
#include "../Util/SOpenGL.h"
#include "framework.h"

static CVector position;
static CVector rotation;

static CVector positionForRender;

static void renderBsp(CBsp* bsp);
static void renderPoly(CPoly* poly);

int main(int argc, char* argv[])
{

	framework.enableDepthBuffer = true;
	
	if (!framework.init(0, nullptr, 640, 480))
		exit(-1);
	
	CMesh A;
	CMesh B;
	CMesh C;

	CGeomBuilder::I().cilinder(A, 3);
	#if 0
	CGeomBuilder::I().matrix.push();
//	CGeomBuilder::I().matrix.rotate(1.2, 1.2, 1.2);
	CGeomBuilder::I().matrix.scale(1.2, 1.2, 1.0);
	CGeomBuilder::I().matrix.translate(0.0, 0.0, 0.5);
	CGeomBuilder::I().cilinder(B, 4);
	CGeomBuilder::I().matrix.pop();
	#else
	CGeomBuilder::I().matrix.push();
	CGeomBuilder::I().matrix.rotate(1.2, 1.2, 1.2);
	CGeomBuilder::I().matrix.scale(1.2, 1.2, 1.2);
	CGeomBuilder::I().sphere(B, 10, 10);
	CGeomBuilder::I().matrix.pop();
	#endif
//	CGeomBuilder::I().cilinder(C, 5);
	CGeomBuilder::I().cube(C);

	CBsp bspA;
	CBsp bspB;
	CBsp bspC;

	#if 1

	A.copy(&bspA);
	B.copy(&bspB);
	C.copy(&bspC);

	#else

	CGeomBuilder::I().cilinder(bspA, 3);
	CGeomBuilder::I().cilinder(bspB, 4);
	CGeomBuilder::I().cilinder(bspC, 5);

	#endif

	#if 0

	for (CPoly* poly = bspA.polyHead; poly; poly = poly->next)
	{
		printf("== poly ==\n");
		printf("normal: %f %f %f\n", poly->plane.normal[0], poly->plane.normal[1], poly->plane.normal[2]);
		for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
		{
			printf("%f %f %f\n", edge->p[0], edge->p[1], edge->p[2]);
		}
	}

	#endif

	printf("Generating BSP trees for primitives...");

	bspA.split();
	bspB.split();
	bspC.split();

	printf("[done]\n");

	CBsp csgA;
	CBsp csgB;
	CBsp csgC;

	printf("Performing CSG operations...");

	CCsg3D::I().addition(&C, &B, &csgA);
	CCsg3D::I().subtraction(&C, &B, &csgB);
	CCsg3D::I().intersection(&C, &B, &csgC);

	printf("[done]\n");

	printf("Generating BSP trees for CSG results...");

	csgA.split();
	csgB.split();
	csgC.split();

	printf("[done]\n");

	position[2] = 4.0;

	while (!keyboard.wentDown(SDLK_ESCAPE))
	{
	
		framework.process();
		
		static float t = 0.0;

		if (keyboard.isDown(SDLK_LEFT))
			position[0] -= 0.05;
		if (keyboard.isDown(SDLK_RIGHT))
			position[0] += 0.05;

		if (keyboard.isDown(SDLK_a))
			position[1] += 0.05;
		if (keyboard.isDown(SDLK_z))
			position[1] -= 0.05;

		if (keyboard.isDown(SDLK_DOWN))
			position[2] += 0.05;
		if (keyboard.isDown(SDLK_UP))
			position[2] -= 0.05;

		rotation[0] = t * 10.0;
		rotation[1] = t * 11.0;
		rotation[2] = t * 13.0;

		static int render = 0;

		if (keyboard.wentDown(SDLK_1))
			render = 0;
		if (keyboard.wentDown(SDLK_2))
			render = 1;
		if (keyboard.wentDown(SDLK_3))
			render = 2;
		if (keyboard.wentDown(SDLK_4))
			render = 3;
		if (keyboard.wentDown(SDLK_5))
			render = 4;
		if (keyboard.wentDown(SDLK_6))
			render = 5;
		
		framework.beginDraw(0, 0, 0, 0);

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(0);

		SOpenGL::I().setupStandardMatrices(0.0);

		gxTranslatef(-position[0], -position[1], -position[2]);
		
		const Mat4x4 r = Mat4x4(true).RotateX(DEG2RAD(t * 10.f)).RotateY(DEG2RAD(t * 10.f)).RotateZ(DEG2RAD(t * 10.f));
		gxMultMatrixf(r.m_v);
		
		const Mat4x4 m = r.CalcInv();
		const Vec3 p = m.Mul4(Vec3(position[0], position[1], position[2]));
		positionForRender = CVector(p[0], p[1], p[2]);

		setBlend(BLEND_ALPHA);

		if (render == 0)
			renderBsp(&bspA);

		if (render == 1)
			renderBsp(&bspB);

		if (render == 2)
			renderBsp(&bspC);

		if (render == 3)
			renderBsp(&csgA);

		if (render == 4)
			renderBsp(&csgB);

		if (render == 5)
			renderBsp(&csgC);

		framework.endDraw();

		t += 0.01;

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
		float d = bsp->plane * positionForRender;
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
	setColorf(tmp[0], tmp[1], tmp[2], .54f);

	gxBegin(GL_TRIANGLE_FAN);
	{

		for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
		{

			gxVertex3f(edge->p[0], edge->p[1], edge->p[2]);

		}

	}
	gxEnd();

}
