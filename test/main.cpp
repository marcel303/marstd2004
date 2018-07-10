#include <allegro.h>
#include "marstd.cpp"
#include "util/SOpenGL.h"

static void renderMesh(CMesh& mesh);
static void isosurfaceOutput(int triangleCount, CIsosurfaceVertex* vertex);

int main(int argc, char* argv[])
{

    allegro_init();
    
    install_keyboard();
    
    if (SOpenGL::I().setGraphicsMode(640, 480, desktop_color_depth(), false) < 0)
        exit(-1);

    // ============
    // CGeomBuilder
    // ============

    {
    
    printf("CGeomBuilder test. Press R to randomize shape.\n");
    
    CMesh mesh;
    
    float t = 0.0;
    
    while (!key[KEY_ESC])
    {
    
        if (key[KEY_R])
        {
        
            mesh.clear();

            int type = rand() % 7;
            
            if (type == 0)
            {
                CGeomBuilder::I().cone(mesh, 32);
                CGeomBuilder::I().cube(mesh);
                CGeomBuilder::I().sphere(mesh, 10, 30);
                printf("->Mixed shape\n");
            }
            if (type == 1)
            {
                CGeomBuilder::I().cilinder(mesh, 10);
                printf("->Cilinder\n");
            }
            if (type == 2)
            {
                CGeomBuilder::I().circle(mesh, 10);
                printf("->Circle\n");
            }
            if (type == 3)
            {
                CGeomBuilder::I().cone(mesh, 10);
                printf("->Cone\n");
            }
            if (type == 4)
            {
                CGeomBuilder::I().cube(mesh);
                printf("->Cube\n");
            }
            if (type == 5)
            {
                CGeomBuilder::I().sphere(mesh, 10, 30);
                printf("->Sphere\n");
            }
            if (type == 6)
            {
                CGeomBuilder::I().donut(mesh, 10, 10, 0.5, 0.25);
                printf("->Sphere\n");
            }            
            
            CCompiledMesh temp;
            CGeomCompiler::I().compile(mesh, temp);
            printf("%d\n", temp.vertex.size());
            
            key[KEY_R] = 0;
                
        }
        
        SOpenGL::I().setupStandardMatrices();
        
        glMatrixMode(GL_MODELVIEW);
        
        glRotatef(t * 1.012, 1.0, 0.0, 0.0);
        glRotatef(t * 1.123, 0.0, 1.0, 0.0);
        glRotatef(t * 1.234, 0.0, 0.0, 1.0);
        
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
		renderMesh(mesh);
        
        SOpenGL::I().flip();
        
        t += 1.0 / 30.0;
        
    }
    
	key[KEY_ESC] = 0;

    }
    
    // ========
    // CBezier3
    // ========
    
    CBezier3 bezier;
    
    {
    
    for (int x = 0; x < 4; ++x)
    {
        for (int y = 0; y < 4; ++y)
        {
            bezier.p[x][y][0] = x;
            bezier.p[x][y][1] = y;
            bezier.p[x][y][2] = 0.0;
        }
    }

    bezier.generate(4);
    
    for (int x = 0; x < bezier.resolution; ++x)
    {
        for (int y = 0; y < bezier.resolution; ++y)
        {
            printf("%f %f %f\n", bezier.v[x][y][0], bezier.v[x][y][1], bezier.v[x][y][2]);
        }
    }
    
    while (!key[KEY_ESC]);

	key[KEY_ESC] = 0;
    
    }
    
    // ===========
    // CIsosurface
    // ===========
    
    {
    
    printf("Isosurface test.\n");
    
    CIsosurface isosurface;
    
    isosurface.setSize(20, 20, 20);

	const int count = 15;

	CIsosurfaceMetaball ball[count];

    CIsosurfaceVertex vertex[300];
    
    float t = 0.0;
    
	while (!key[KEY_ESC])
	{
	
		for (int i = 0; i < count; ++i)
		{
		
			float speed = (i + 1) / 10.0;

			ball[i].p[0] = sin(t * speed * 1.012) * 0.7;
			ball[i].p[1] = cos(t * speed * 1.123) * 0.7;
			ball[i].p[2] = (sin(t * speed * 1.234) + cos(t * speed * 1.345)) * 0.7 * 0.5;
		
			ball[i].a = 0.05;

		}
		
		isosurface.calculate(ball, count);
		
		isosurface.calculateNormals();
		
        SOpenGL::I().setupStandardMatrices();

        glMatrixMode(GL_MODELVIEW);

        glRotatef(t * 1.012, 1.0, 0.0, 0.0);
        glRotatef(t * 1.123, 0.0, 1.0, 0.0);
        glRotatef(t * 1.234, 0.0, 0.0, 1.0);

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		isosurface.output(100, vertex, isosurfaceOutput);

        SOpenGL::I().flip();
        
		t += 1.0 / 30.0;
	
	}
    
	key[KEY_ESC] = 0;
    
    }
    
    return 0;

}
END_OF_MAIN();

static void renderMesh(CMesh& mesh)
{

    for (CPoly* poly = mesh.polyHead; poly; poly = poly->next)
    {

        glColor3ub(
            int((poly->plane.normal[0] + 1.0) * 0.5 * 255),
            int((poly->plane.normal[1] + 1.0) * 0.5 * 255),
            int((poly->plane.normal[2] + 1.0) * 0.5 * 255));

        glBegin(GL_POLYGON);
        {
            for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
            {
                glVertex3fv(edge->p);
            }
        }
        glEnd();
        
        glColor3ub(255, 255, 255);
        
        glBegin(GL_LINES);
        {
            for (CEdge* edge = poly->edgeHead; edge; edge = edge->next)
            {
            	CVector p1 = edge->p;
            	CVector p2 = edge->p + poly->plane.normal * 0.1;
                glVertex3fv(p1);            	
                glVertex3fv(p2);
            }
        }
        glEnd();        
        
    }
        
}

static void isosurfaceOutput(int triangleCount, CIsosurfaceVertex* vertex)
{

	glBegin(GL_TRIANGLES);
	{
	
		for (int i = 0; i < triangleCount; ++i)
		{
		
			for (int j = 0; j < 3; ++j)
			{

				glColor3fv(vertex[i * 3 + j].n);
				
				glVertex3fv(vertex[i * 3 + j].p);
			
			}
		
		}
	
	}
	glEnd();
	
}
