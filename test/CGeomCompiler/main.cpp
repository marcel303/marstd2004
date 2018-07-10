#include <allegro.h>
#include "marstd.cpp"
#include "../util/SOpenGL.h"
#include "../util/SOpenGLTextureManager.h"

static void generateTextureCoordinates(CMesh& mesh);
static void generateTextureCoordinates(CPoly& poly);

int main(int argc, char* argv[])
{

	allegro_init();
	
	if (install_keyboard() < 0)
		exit(-1);
		
	CMesh mesh, mesh2;
	
	CGeomBuilder::I().cube(mesh);
	CGeomBuilder::I().cube(mesh2);
	CGeomBuilder::I().sphere(mesh, 10, 20);
	CGeomBuilder::I().sphere(mesh2, 10, 20);
	CGeomBuilder::I().cone(mesh, 10);
	CGeomBuilder::I().cone(mesh2, 10);
	
	generateTextureCoordinates(mesh);
	generateTextureCoordinates(mesh2);

	CCompiledMesh base, compiledMesh, compiledMesh2;

	CGeomCompiler::I().compile(mesh, base, compiledMesh);
	CGeomCompiler::I().compile(mesh2, base, compiledMesh2);
	
	printf("vertex_count: %d\n", base.vertex.size());
	printf("edge_count: %d\n", base.edge.size());
	printf("poly_count: %d\n", base.polygon.size());
	
	#if 0
	for (int i = 0; i < base.vertex.size(); ++i)
	{
		printf("vertex %d\n", i);
		for (int j = 0; j < base.vertex[i].edge.size(); ++j)
		{
			printf("-> edge %d\n", base.vertex[i].edge[j]);
		}
		for (int j = 0; j < base.vertex[i].polygon.size(); ++j)
		{
			printf("-> polygon %d\n", base.vertex[i].polygon[j]);
		}
	}
	#endif
	
	#if 0
	for (int i = 0; i < compiledMesh.edge.size(); ++i)
	{
		printf("edge %d\n", i);
		for (int j = 0; j < compiledMesh.edge[i].vertex.size(); ++j)
		{
			printf("-> vertex %d\n", compiledMesh.edge[i].vertex[j]);
		}
	}
	#endif
	
	#if 0
	for (int i = 0; i < (int)compiledMesh.polygon.size(); ++i)
	{
		printf("polygon %d\n", i);
		for (int j = 0; j < (int)compiledMesh.polygon[i].edge.size(); ++j)
		{
			printf("-> edge %d -> vertex %d\n", compiledMesh.polygon[i].edge[j], compiledMesh.polygon[i].vertex[j]);
		}
	}
	#endif
	
//	readkey();
	
	if (SOpenGL::I().setGraphicsMode(640, 480, desktop_color_depth(), false) < 0)
		exit(-1);

	GLuint texturemap = SOpenGLTextureManager::I().addTextureFromFile("../data/texture1.bmp");

	if (texturemap == 0)
		exit(-1);

  	float time = 0.0;
  	
	while (!key[KEY_ESC])
	{
	
  		if (key[KEY_R])
  		{
  		
  			CMesh mesh;
  			
  			float scale = (rand()&1023) / 1023.0 + 0.5;
			CGeomBuilder::I().matrix.identity();
			CGeomBuilder::I().matrix.scale(scale, scale, scale);
			CGeomBuilder::I().cube(mesh);
  			scale = (rand()&1023) / 1023.0 + 0.5;
			CGeomBuilder::I().matrix.identity();
			CGeomBuilder::I().matrix.scale(scale, scale, scale);
			CGeomBuilder::I().sphere(mesh, 10, 20);
  			scale = (rand()&1023) / 1023.0 + 0.5;
			CGeomBuilder::I().matrix.identity();
			CGeomBuilder::I().matrix.scale(scale, scale, scale);
			CGeomBuilder::I().cone(mesh, 10);

			generateTextureCoordinates(mesh);

			base.clear();
			compiledMesh.clear();
			
			CGeomCompiler::I().compile(mesh, base, compiledMesh);
			
		}
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(0);
		
		glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
		glEnable(GL_BLEND);
		
		SOpenGL::I().setupStandardMatrices();
		
		glRotatef(time / 1.123, 1.0, 0.0, 0.0);
		glRotatef(time / 1.234, 0.0, 1.0, 0.0);
		glRotatef(time / 1.345, 0.0, 0.0, 1.0);

		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		#if 0
		
		for (int i = 0; i < 4; ++i)
		{
			char temp[256];
			sprintf(temp, "[ %1.3f ] [ %1.3f ] [ %1.3f ] [ %1.3f ]\n", matrix[0 * 4 + i], matrix[1 * 4 + i], matrix[2 * 4 + i], matrix[3 * 4 + i]);
			printf(temp);
		}
		printf("\n");
		
		#endif

		#if 1
  		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		#endif
		
		// Polygons.

		glBindTexture(GL_TEXTURE_2D, texturemap);
		glEnable(GL_TEXTURE_2D);

		for (int i = 0; i < (int)compiledMesh.polygonIndex.size(); ++i)
		{
			glColor3ub(255, 255, 255);
      		glBegin(GL_POLYGON);
      		{
      			int p = compiledMesh.polygonIndex[i];
      			for (int j = 0; j < (int)base.polygon[p].vertex.size(); ++j)
      			{
      				float v[4] =
      				{
      					base.vertex[base.polygon[p].vertex[j]].p[0],
      					base.vertex[base.polygon[p].vertex[j]].p[1],
      					base.vertex[base.polygon[p].vertex[j]].p[2],
      					1.0
      				};
      				float n[4] =
      				{
      					base.vertex[base.polygon[p].vertex[j]].plane.normal[0],
      					base.vertex[base.polygon[p].vertex[j]].plane.normal[1],
      					base.vertex[base.polygon[p].vertex[j]].plane.normal[2],
      					0.0
      				};
      				float v_t[4];
      				float n_t[4];
      				for (int i = 0; i < 4; ++i)
      				{
      					v_t[i] = 0.0;
      					n_t[i] = 0.0;
      					for (int j = 0; j < 4; ++j)
      					{
      						v_t[i] += v[j] * matrix[i + j * 4];
      						n_t[i] += n[j] * matrix[i + j * 4];
      					}
      				}
//   					glColor3fv(n);
					glTexCoord2f(base.texcoord[base.polygon[p].texcoord[j]].t[0], base.texcoord[base.polygon[p].texcoord[j]].t[1]);
      				glNormal3fv(n_t);
      				glVertex4fv(v_t);
      			}
      		}
      		glEnd();
		}

		glDisable(GL_TEXTURE_2D);
		
 		// Normals at vertices.

  		glColor3ub(0, 255, 0);
  		glBegin(GL_LINES);
  		{
  			for (int i = 0; i < (int)base.vertex.size(); ++i)
  			{
  				float v1[4] =
  				{
  					base.vertex[i].p[0],
  					base.vertex[i].p[1],
  					base.vertex[i].p[2],
					1.0
  				};
  				float v2[4] =
  				{
  					base.vertex[i].p[0] + base.vertex[i].plane.normal[0] * 0.1,
  					base.vertex[i].p[1] + base.vertex[i].plane.normal[1] * 0.1,
  					base.vertex[i].p[2] + base.vertex[i].plane.normal[2] * 0.1,
					1.0
  				};
  				float v1_t[4];
  				float v2_t[4];
  				for (int i = 0; i < 4; ++i)
  				{
  					v1_t[i] = 0.0;
  					v2_t[i] = 0.0;
  					for (int j = 0; j < 4; ++j)
  					{
  						v1_t[i] += v1[j] * matrix[i + j * 4];
  						v2_t[i] += v2[j] * matrix[i + j * 4];
  					}
  				}
  				glVertex4fv(v1_t);
  				glVertex4fv(v2_t);
  			}
  		}
  		glEnd();
  		
		// Polygon normals.

  		glColor3ub(0, 0, 255);
  		glBegin(GL_LINES);
  		{
  			for (int i = 0; i < (int)base.polygon.size(); ++i)
  			{
  				float v1[4] =
  				{
  					base.polygon[i].center[0],
  					base.polygon[i].center[1],
  					base.polygon[i].center[2],
					1.0
  				};
  				float v2[4] =
  				{
  					base.polygon[i].center[0] + base.polygon[i].plane.normal[0] * 0.1,
  					base.polygon[i].center[1] + base.polygon[i].plane.normal[1] * 0.1,
  					base.polygon[i].center[2] + base.polygon[i].plane.normal[2] * 0.1,
					1.0
  				};
  				float v1_t[4];
  				float v2_t[4];
  				for (int i = 0; i < 4; ++i)
  				{
  					v1_t[i] = 0.0;
  					v2_t[i] = 0.0;
  					for (int j = 0; j < 4; ++j)
  					{
  						v1_t[i] += v1[j] * matrix[i + j * 4];
  						v2_t[i] += v2[j] * matrix[i + j * 4];
  					}
  				}
  				glVertex4fv(v1_t);
  				glVertex4fv(v2_t);
  			}
  		}
  		glEnd();
		
		SOpenGL::I().flip();
		
		time += 1.0 / 10.0;
	
	}

	return 0;

} END_OF_MAIN();

static void generateTextureCoordinates(CMesh& mesh)
{

	for (CPoly* poly = mesh.polyHead; poly; poly = poly->next)
		generateTextureCoordinates(*poly);

}

static void generateTextureCoordinates(CPoly& poly)
{

	// Calculate texture coordinates.

	const float uv[3][2][3] =
	{
		{
			{ 0.0, 0.0, 1.0 },
			{ 0.0, 1.0, 0.0 }
		},
		{
			{ 0.0, 0.0, 1.0 },
			{ 1.0, 0.0, 0.0 }
		},
		{
			{ 1.0, 0.0, 0.0 },
			{ 0.0, 1.0, 0.0 }
		}
	};

	// Find best texture plane.

	int plane;
	if (ABS(poly.plane.normal[0]) > ABS(poly.plane.normal[1]))
		plane = 0;
	else
		plane = 1;
	if (ABS(poly.plane.normal[2]) > ABS(poly.plane.normal[plane]))
		plane = 2;

	for (CEdge* edge = poly.edgeHead; edge; edge = edge->next)
	{

		// Apply texture plane to get texture coordinates.

		for (int i=0; i<2; i++)
			edge->t[0][i] = (uv[plane][i][0] * edge->p[0] + uv[plane][i][1] * edge->p[1] + uv[plane][i][2] * edge->p[2]);

	}

}
