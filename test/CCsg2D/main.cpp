#include "marstd.h"
#include "framework.h"
#include <SDL2/SDL_messagebox.h>

class Polydata
{
	public:
	Polydata()
	{
		pressed = 0;
		lightness = 0;
	}
	public:
	int pressed;
	int lightness;
};

static void renderPoly(CPoly* poly, Color color = colorBlackTranslucent);
static void renderMesh(CMesh* mesh, Color color = colorBlackTranslucent);
static bool insidePoly(CPoly* poly, int x, int y);

int main(int argc, char* argv[])
{

#if defined(CHIBI_RESOURCE_PATH)
	changeDirectory(CHIBI_RESOURCE_PATH);
#endif
	
	if (!framework.init(400, 400))
		exit(-1);
	
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Instructions.", "Press 1-4 to move through all 2D CSG modes.", nullptr);
	
	CVector screenMid = CVector(400 / 2.0, 400 / 2.0, 0.0);

	CPoly* poly1 = new CPoly;
	for (int i = 0; i < 4; ++i)
	{
		CEdge* edge = poly1->add(new CEdge);
		edge->p = screenMid;
		edge->p[0] += sin(i / 4.0 * 2.0 * M_PI) * 50.0 - 25;
		edge->p[1] += cos(i / 4.0 * 2.0 * M_PI) * 50.0;
	}
	poly1->init();

	CPoly* poly2 = new CPoly;
	for (int i = 0; i < 6; ++i)
	{
		CEdge* edge = poly2->add(new CEdge);
		edge->p = screenMid;
		edge->p[0] += sin(i / 6.0 * 2.0 * M_PI) * 50.0 + 25;
		edge->p[1] += cos(i / 6.0 * 2.0 * M_PI) * 50.0;
	}
	poly2->init();

	CMesh* addition = new CMesh;
	CCsg2D::I().addition(poly1, poly2, addition);

	CMesh* subtraction = new CMesh;
	CCsg2D::I().subtraction(poly1, poly2, subtraction);

	CPoly* intersection = new CPoly;
	CCsg2D::I().intersection(poly1, poly2, intersection);

	CMesh* exclusiveor = new CMesh;
	CCsg2D::I().exclusiveor(poly1, poly2, exclusiveor);

	// Add polydata to every polygon...

	for (CPoly* poly = addition->polyHead; poly; poly = poly->next)
		poly->data = new Polydata;
	for (CPoly* poly = subtraction->polyHead; poly; poly = poly->next)
		poly->data = new Polydata;
	intersection->data = new Polydata;
	for (CPoly* poly = exclusiveor->polyHead; poly; poly = poly->next)
		poly->data = new Polydata;
	
	while (!keyboard.wentDown(SDLK_ESCAPE))
	{

		framework.process();
		
		static int type = 0;
		
		if (keyboard.wentDown(SDLK_1))
			type = 0;
		if (keyboard.wentDown(SDLK_2))
			type = 1;
		if (keyboard.wentDown(SDLK_3))
			type = 2;
		if (keyboard.wentDown(SDLK_4))
			type = 3;

		// Render.

		framework.beginDraw(0, 0, 0, 0);

		if (type == 0)
			renderMesh(addition);
		else if (type == 1)
			renderMesh(subtraction);
		else if (type == 2)
			renderPoly(intersection);
		else if (type == 3)
			renderMesh(exclusiveor);

		const char* desc[4] =
		{
			"CSG 2D: Addition",
			"CSG 2D: Subtraction",
			"CSG 2D: Intersection",
			"CSG 2D: Exclusive or",
		};
		
		setFont("../Data/calibri.ttf");
		setColor(colorWhite);
		drawText(5, 5, 14, +1, +1, "%s", desc[type]);

		framework.endDraw();

	}
	
	framework.shutdown();

	return 0;

}

static void printPoly(CPoly* poly)
{

	printf("=== Poly ===\n");

	printf("Normal: %f %f %f %f\n", poly->plane.normal[0], poly->plane.normal[1], poly->plane.normal[2], poly->plane.distance);

	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
		printf("Edge: %f %f %f\n", edge->p[0], edge->p[1], edge->p[2]);

}

static void renderPoly(CPoly* poly, Color color)
{

	// Update polygon's state.

	Polydata* polydata = (Polydata*)poly->data;

	if (insidePoly(poly, mouse.x, mouse.y))
	{
		if (mouse.isDown(BUTTON_LEFT))
		{
			if (!polydata->pressed)
				printPoly(poly);
			polydata->pressed = 1;
			polydata->lightness = 255;
		}
		else
		{
			polydata->pressed = 0;
			if (polydata->lightness < 127)
			{
				polydata->lightness++;
				if (polydata->lightness > 127)
					polydata->lightness = 127;
			}
			else
			{
				polydata->lightness--;
				if (polydata->lightness < 127)
					polydata->lightness = 127;
			}
		}
	}
	else
	{
		polydata->pressed = 0;
		polydata->lightness--;
		if (polydata->lightness < 63)
			polydata->lightness = 63;
	}
	
	if (color.a == 0.f)
		color = colorWhite;

	int* points = new int[poly->edgeCount * 2];
	int point = 0;
	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
	{
		points[point * 2 + 0] = (int)edge->p[0];
		points[point * 2 + 1] = (int)edge->p[1];
		point++;
	}
	Color fillColor = Color(polydata->lightness, polydata->lightness, polydata->lightness);
	setColor(fillColor);
	gxBegin(GX_TRIANGLE_FAN);
	for (int i = 0; i < poly->edgeCount; ++i)
	{
		gxVertex2f(points[i * 2 + 0], points[i * 2 + 1]);
	}
	gxEnd();
	delete[] points;

	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
	{

		const float normalSize = polydata->lightness / 255.0 * 20.0;

		float edgeMid[2];

		edgeMid[0] = (edge->p[0] + edge->edge2->p[0]) / 2.0;
		edgeMid[1] = (edge->p[1] + edge->edge2->p[1]) / 2.0;

		// Vertex to vertex.
		
		setColor(color);
		drawLine((int)edge->p[0], (int)edge->p[1], (int)edge->edge2->p[0], (int)edge->edge2->p[1]);

		// Plane || edge.
		
		setColor(0, 255, 0);
		drawLine((int)edgeMid[0], (int)edgeMid[1], int(edgeMid[0] + edge->edge_plane.normal[0] * normalSize), int(edgeMid[1] + edge->edge_plane.normal[1] * normalSize));

		// Plane _|_ edge.
		
		setColor(255, 255, 0);
		drawLine((int)edgeMid[0], (int)edgeMid[1], int(edgeMid[0] + edge->plane.normal[0] * normalSize), int(edgeMid[1] + edge->plane.normal[1] * normalSize));

	}

}

static void renderMesh(CMesh* mesh, Color color)
{

	for (CPoly* poly = mesh->polyHead; poly; poly = poly->next)
		renderPoly(poly, color);

}

static bool insidePoly(CPoly* poly, int x, int y)
{

	CVector position = CVector(x, y, 0.0);

	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
		if (edge->plane * position > 0.0)
			return false;

	return true;

}
