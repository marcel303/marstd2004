#include <allegro.h>
#include "marstd.cpp"

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

static BITMAP* colourmap = 0;

static void renderPoly(CPoly* poly, int colour = -1);
static void renderMesh(CMesh* mesh, int colour = -1);
static bool insidePoly(CPoly* poly, int x, int y);

int main(int argc, char* argv[])
{

	allegro_init();
	
	allegro_message("Press 1-4 to move through all 2D CSG modes.");
	
	install_keyboard();
	install_mouse();
	
	set_color_depth(32);

	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 400, 400, 0, 0);

	colourmap = create_bitmap(SCREEN_W, SCREEN_H);

	CVector screenMid = CVector(SCREEN_W / 2.0, SCREEN_H / 2.0, 0.0);

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

	show_mouse(screen);

	while (!key[KEY_ESC])
	{

		static int type = 0;

		while (keypressed())
		{
			int c = readkey() >> 8;
			if (c == KEY_1)
				type = 0;
			if (c == KEY_2)
				type = 1;
			if (c == KEY_3)
				type = 2;
			if (c == KEY_4)
				type = 3;
		}

		// Render.

		clear(colourmap);

		if (type == 0)
			renderMesh(addition);
		else if (type == 1)
			renderMesh(subtraction);
		else if (type == 2)
			renderPoly(intersection);
		else if (type == 3)
			renderMesh(exclusiveor);

		char* desc[4] =
		{
			"CSG 2D: Addition",
			"CSG 2D: Subtraction",
			"CSG 2D: Intersection",
			"CSG 2D: Exclusive or",
		};

		textprintf_ex(colourmap, font, 5, 5, makecol(255, 255, 255), makecol(0, 0, 0), desc[type]);

		// Put colourmap on screen.

		vsync();

		scare_mouse();

		blit(colourmap, screen, 0, 0, 0, 0, colourmap->w, colourmap->h);

		unscare_mouse();

	}

	destroy_bitmap(colourmap);

	return 0;

}
END_OF_MAIN();

static void printPoly(CPoly* poly)
{

	printf("=== Poly ===\n");

	printf("Normal: %f %f %f %f\n", poly->plane.normal[0], poly->plane.normal[1], poly->plane.normal[2], poly->plane.distance);

	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
		printf("Edge: %f %f %f\n", edge->p[0], edge->p[1], edge->p[2]);

}

static void renderPoly(CPoly* poly, int colour)
{

	// Update polygon's state.

	Polydata* polydata = (Polydata*)poly->data;

	if (insidePoly(poly, mouse_x, mouse_y))
	{
		if (mouse_b & 1)
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
	int fillColour = makecol(polydata->lightness, polydata->lightness, polydata->lightness);
	polygon(colourmap, poly->edgeCount, points, fillColour);
	delete[] points;

	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
	{

		const float normalSize = polydata->lightness / 255.0 * 20.0;

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

static void renderMesh(CMesh* mesh, int colour)
{

	for (CPoly* poly = mesh->polyHead; poly; poly = poly->next)
		renderPoly(poly, colour);

}

static bool insidePoly(CPoly* poly, int x, int y)
{

	CVector position = CVector(x, y, 0.0);

	for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
		if (edge->plane * position > 0.0)
			return false;

	return true;

}
