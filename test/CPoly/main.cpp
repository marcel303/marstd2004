#include <allegro.h>
#include "marstd.cpp"

static BITMAP* colourmap;

static void renderPoly(CPoly* poly, int colour = -1);
static bool insidePoly(CPoly* poly, int x, int y);

int main(int argc, char* argv[])
{

	allegro_init();
	
	install_keyboard();
	install_mouse();
	
	set_color_depth(32);

	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 400, 400, 0, 0);

	colourmap = create_bitmap(SCREEN_W, SCREEN_H);

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

	show_mouse(screen);

	while (!key[KEY_ESC])
	{

		// Render.

		clear(colourmap);

		int line = 0;

		textprintf_ex(colourmap, font, 0, line++ * text_height(font), makecol(255, 255, 255), makecol(0, 0, 0), "Press Q to hide original polygon.");
		if (!key[KEY_Q])
			renderPoly(poly);

		textprintf_ex(colourmap, font, 0, line++ * text_height(font), makecol(255, 255, 255), makecol(0, 0, 0), "Press W to render split polygon.");
		if (key[KEY_W])
		{
			renderPoly(front);
			renderPoly(back);
		}

		textprintf_ex(colourmap, font, 0, line++ * text_height(font), makecol(255, 255, 255), makecol(0, 0, 0), "Press E to render clipped polygon.");
		if (key[KEY_E])
			renderPoly(clip);

		textprintf_ex(colourmap, font, 0, line++ * text_height(font), makecol(255, 255, 255), makecol(0, 0, 0), "Press R to render clipping polygon.");
		if (key[KEY_R])
			renderPoly(clipPoly);

		textprintf_ex(colourmap, font, 0, line++ * text_height(font), makecol(255, 255, 255), makecol(0, 0, 0), "Press T to render inside polygon.");
		if (key[KEY_T])
			renderPoly(inside);

		textprintf_ex(colourmap, font, 0, line++ * text_height(font), makecol(255, 255, 255), makecol(0, 0, 0), "Press Y to render outside polygon.");
		if (key[KEY_Y])
			for (CPoly* poly = outside->polyHead; poly; poly = poly->next)
				renderPoly(poly);

		vsync();

		scare_mouse();

		blit(colourmap, screen, 0, 0, 0, 0, colourmap->w, colourmap->h);

		unscare_mouse();

	}

	destroy_bitmap(colourmap);

	return 0;

}
END_OF_MAIN();

static void renderPoly(CPoly* poly, int colour)
{

	if (colour == -1)
		colour = makecol(255, 255, 255);

	int* points = new int[poly->edgeCount * 2];
	int point = 0;
	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
	{
		points[point * 2 + 0] = (int)edge->p[0];
		points[point * 2 + 1] = (int)edge->p[1];
		point++;
	}
	int fillColour = insidePoly(poly, mouse_x, mouse_y) ? makecol(127, 127, 127) : makecol(63, 63, 63);
	polygon(colourmap, poly->edgeCount, points, fillColour);
	delete[] points;

	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
	{

		const float normalSize = 50.0;

		float edgeMid[2];

		edgeMid[0] = (edge->p[0] + edge->edge2->p[0]) / 2.0;
		edgeMid[1] = (edge->p[1] + edge->edge2->p[1]) / 2.0;

		// Vertex to vertex.

		line(colourmap, (int)edge->p[0], (int)edge->p[1], (int)edge->edge2->p[0], (int)edge->edge2->p[1], colour);

		// Plane || edge.

		line(colourmap, (int)edgeMid[0], (int)edgeMid[1], int(edgeMid[0] + edge->edge_plane.normal[0] * normalSize), int(edgeMid[1] + edge->edge_plane.normal[1] * normalSize), makecol(0, 255, 0));

		// Plane _|_ edge.

		line(colourmap, (int)edgeMid[0], (int)edgeMid[1], int(edgeMid[0] + edge->plane.normal[0] * normalSize), int(edgeMid[1] + edge->plane.normal[1] * normalSize), makecol(255, 255, 0));

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
