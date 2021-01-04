#include "framework.h"
#include "marstd.cpp"

static const int SCREEN_W = 400;
static const int SCREEN_H = 400;

static void renderPoly(CPoly* poly, Color * colour = nullptr);
static bool insidePoly(CPoly* poly, int x, int y);

int main(int argc, char* argv[])
{

	setupPaths(CHIBI_RESOURCE_PATHS);
	
	framework.init(SCREEN_W, SCREEN_H);
	
	CVector screenMid = CVector(SCREEN_W / 2.0, SCREEN_H / 2.0, 0.0);

	CPoly* poly = new CPoly;

	for (int i = 0; i < 5; ++i)
	{

		CEdge* edge = poly->add(new CEdge);

		edge->p[0] = sin(i / 5.0 * 2.0 * M_PI) * 100 + SCREEN_W / 2.0;
		edge->p[1] = cos(i / 5.0 * 2.0 * M_PI) * 100 + SCREEN_H / 2.0;
		edge->p[2] = 0.0;

	}

	poly->init();

	CPoly* front = new CPoly;
	CPoly* back = new CPoly;

	CPlane splitPlane;
	splitPlane.normal[0] = 1.0;
	splitPlane.normal[1] = 1.0;
	splitPlane.normal[2] = 0.0;
	splitPlane.normal.normalize();
	splitPlane.distance = splitPlane.normal * screenMid;

	poly->clip(&splitPlane, front, back);

 	CPlane clipPlane;
	clipPlane.normal[0] = 0.0;
	clipPlane.normal[1] = 1.0;
	clipPlane.normal[2] = 0.0;
	clipPlane.distance = clipPlane.normal * screenMid;

	CPoly* clip = poly->clip(&clipPlane);

	CPoly* inside = new CPoly;
	CMesh* outside = new CMesh;
	CPoly* clipPoly = new CPoly;
	for (int i = 0; i < 5; ++i)
  	{

		CEdge* edge = clipPoly->add(new CEdge);

		edge->p[0] = sin(i / 5.0 * 2.0 * M_PI) * 150 + SCREEN_W / 2.0 - 100;
		edge->p[1] = cos(i / 5.0 * 2.0 * M_PI) * 150 + SCREEN_H / 2.0;

		edge->p[2] = 0.0;

	}
	clipPoly->init();

	clipPoly->clip(poly, inside, outside);

	while (!keyboard.wentDown(SDLK_ESCAPE))
	{

		framework.process();
		
		// Render.

		framework.beginDraw(0, 0, 0, 0);

		setFont("engine/fonts/Roboto-Regular.ttf");
		
		int line = 0;
		int text_height = 18;

		setColor(colorWhite);
		drawText(0, line++ * text_height, 12, +1, +1, "Press Q to hide original polygon.");
		if (!keyboard.isDown(SDLK_q))
			renderPoly(poly);

		setColor(colorWhite);
		drawText(0, line++ * text_height, 12, +1, +1, "Press W to render split polygon.");
		if (keyboard.isDown(SDLK_w))
		{
			renderPoly(front);
			renderPoly(back);
		}

		setColor(colorWhite);
		drawText(0, line++ * text_height, 12, +1, +1, "Press E to render clipped polygon.");
		if (keyboard.isDown(SDLK_e))
			renderPoly(clip);

		setColor(colorWhite);
		drawText(0, line++ * text_height, 12, +1, +1, "Press R to render clipping polygon.");
		if (keyboard.isDown(SDLK_r))
			renderPoly(clipPoly);

		setColor(colorWhite);
		drawText(0, line++ * text_height, 12, +1, +1, "Press T to render inside polygon.");
		if (keyboard.isDown(SDLK_t))
			renderPoly(inside);

		setColor(colorWhite);
		drawText(0, line++ * text_height, 12, +1, +1, "Press Y to render outside polygon.");
		if (keyboard.isDown(SDLK_y))
			for (CPoly* poly = outside->polyHead; poly; poly = poly->next)
				renderPoly(poly);

		framework.endDraw();
		
	}

	framework.shutdown();

	return 0;

}

static void drawHqLine(float x1, float y1, float x2, float y2)
{

	hqBegin(HQ_LINES);
	{
	
		hqLine(x1, y1, 1.f, x2, y2, 1.f);
	
	}
	hqEnd();
	
}

static void renderPoly(CPoly* poly, Color * colour)
{

	if (colour == nullptr)
		colour = &colorWhite;

	bool isInsidePoly = insidePoly(poly, mouse.x, mouse.y);
	
	Color fillColour = isInsidePoly ? Color(127, 127, 127) : Color(63, 63, 63);
	setColor(fillColour);
	gxBegin(GX_TRIANGLE_FAN);
	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
		gxVertex2f(edge->p[0], edge->p[1]);
	gxEnd();

	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
	{

		const float normalSize = (isInsidePoly && mouse.isDown(BUTTON_LEFT)) ? 40.0 : 50.0;

		float edgeMid[2];

		edgeMid[0] = (edge->p[0] + edge->edge2->p[0]) / 2.0;
		edgeMid[1] = (edge->p[1] + edge->edge2->p[1]) / 2.0;

		// Vertex to vertex.

		setColor(*colour);
		drawHqLine(
			edge->p[0],
			edge->p[1],
			edge->edge2->p[0],
			edge->edge2->p[1]);

		// Plane || edge.

		setColor(Color(0, 255, 0));
		drawHqLine(
			edgeMid[0],
			edgeMid[1],
			edgeMid[0] + edge->edge_plane.normal[0] * normalSize,
			edgeMid[1] + edge->edge_plane.normal[1] * normalSize);

		// Plane _|_ edge.

		setColor(Color(255, 255, 0));
		drawHqLine(
			edgeMid[0],
			edgeMid[1],
			edgeMid[0] + edge->plane.normal[0] * normalSize,
			edgeMid[1] + edge->plane.normal[1] * normalSize);

	}

}

static bool insidePoly(CPoly* poly, int x, int y)
{

	CVector position = CVector(x, y, 0.0);

	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
		if (edge->plane * position > 0.0)
			return false;

	return true;

}
