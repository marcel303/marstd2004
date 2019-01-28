#include <GL/glew.h> // glBlendFunc
#include "marstd.h"
#include "../Util/SOpenGL.h"
#include "framework.h"

static void generateTextureCoordinates(CMesh& mesh);
static void generateTextureCoordinates(CPoly& poly);

int main(int argc, char* argv[])
{

#if defined(CHIBI_RESOURCE_PATH)
	changeDirectory(CHIBI_RESOURCE_PATH);
#endif

	framework.enableDepthBuffer = true;
	
	if (!framework.init(640, 480))
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
	
	printf("vertex_count: %lu\n", base.vertex.size());
	printf("edge_count: %lu\n", base.edge.size());
	printf("poly_count: %lu\n", base.polygon.size());
	
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
	
	GxTextureId texturemap = getTexture("../data/texture1.bmp");

	if (texturemap == 0)
		exit(-1);

  	float time = 0.f;
  	
	while (!keyboard.wentDown(SDLK_ESCAPE))
	{
		framework.process();
		
  		if (keyboard.isDown(SDLK_r))
  		{
  		
  			CMesh mesh;
  			
  			float scale = (rand()&1023) / 1023.f + 0.5f;
			CGeomBuilder::I().matrix.identity();
			CGeomBuilder::I().matrix.scale(scale, scale, scale);
			CGeomBuilder::I().cube(mesh);
  			scale = (rand()&1023) / 1023.f + 0.5f;
			CGeomBuilder::I().matrix.identity();
			CGeomBuilder::I().matrix.scale(scale, scale, scale);
			CGeomBuilder::I().sphere(mesh, 10, 20);
  			scale = (rand()&1023) / 1023.f + 0.5f;
			CGeomBuilder::I().matrix.identity();
			CGeomBuilder::I().matrix.scale(scale, scale, scale);
			CGeomBuilder::I().cone(mesh, 10);

			generateTextureCoordinates(mesh);

			base.clear();
			compiledMesh.clear();
			
			CGeomCompiler::I().compile(mesh, base, compiledMesh);
			
		}
	
		framework.beginDraw(40, 40, 40, 0);
		
		setDepthTest(true, DEPTH_LEQUAL, false);
		
		glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
		glEnable(GL_BLEND);
		
		SOpenGL::I().setupStandardMatrices();
		
		gxRotatef(time / 1.123 * 3.0, 1.f, 0.f, 0.f);
		gxRotatef(time / 1.234 * 3.0, 0.f, 1.f, 0.f);
		gxRotatef(time / 1.345 * 3.0, 0.f, 0.f, 1.f);

		float matrix[16];
		gxGetMatrixf(GX_MODELVIEW, matrix);
		gxMatrixMode(GX_MODELVIEW);
		gxLoadIdentity();
		
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
  		
		pushWireframe(true);
		
		#endif
		
		// Polygons.

		gxSetTexture(texturemap);
		
		for (int i = 0; i < (int)compiledMesh.polygonIndex.size(); ++i)
		{
			gxColor3ub(255, 255, 255);
      		gxBegin(GX_TRIANGLE_FAN);
      		{
      			int p = compiledMesh.polygonIndex[i];
      			for (int j = 0; j < (int)base.polygon[p].vertex.size(); ++j)
      			{
      				float v[4] =
      				{
      					base.vertex[base.polygon[p].vertex[j]].p[0],
      					base.vertex[base.polygon[p].vertex[j]].p[1],
      					base.vertex[base.polygon[p].vertex[j]].p[2],
      					1.f
      				};
      				float n[4] =
      				{
      					base.vertex[base.polygon[p].vertex[j]].plane.normal[0],
      					base.vertex[base.polygon[p].vertex[j]].plane.normal[1],
      					base.vertex[base.polygon[p].vertex[j]].plane.normal[2],
      					0.f
      				};
      				float v_t[4];
      				float n_t[4];
      				for (int i = 0; i < 4; ++i)
      				{
      					v_t[i] = 0.f;
      					n_t[i] = 0.f;
      					for (int j = 0; j < 4; ++j)
      					{
      						v_t[i] += v[j] * matrix[i + j * 4];
      						n_t[i] += n[j] * matrix[i + j * 4];
      					}
      				}
//   					gxColor3fv(n);
					gxTexCoord2f(base.texcoord[base.polygon[p].texcoord[j]].t[0], base.texcoord[base.polygon[p].texcoord[j]].t[1]);
      				gxNormal3fv(n_t);
      				gxVertex4fv(v_t);
      			}
      		}
      		gxEnd();
		}

		gxSetTexture(0);
		
 		// Normals at vertices.

  		gxColor3ub(0, 255, 0);
  		gxBegin(GX_LINES);
  		{
  			for (int i = 0; i < (int)base.vertex.size(); ++i)
  			{
  				float v1[4] =
  				{
  					base.vertex[i].p[0],
  					base.vertex[i].p[1],
  					base.vertex[i].p[2],
					1.f
  				};
  				float v2[4] =
  				{
  					base.vertex[i].p[0] + base.vertex[i].plane.normal[0] * 0.1f,
  					base.vertex[i].p[1] + base.vertex[i].plane.normal[1] * 0.1f,
  					base.vertex[i].p[2] + base.vertex[i].plane.normal[2] * 0.1f,
					1.f
  				};
  				float v1_t[4];
  				float v2_t[4];
  				for (int i = 0; i < 4; ++i)
  				{
  					v1_t[i] = 0.f;
  					v2_t[i] = 0.f;
  					for (int j = 0; j < 4; ++j)
  					{
  						v1_t[i] += v1[j] * matrix[i + j * 4];
  						v2_t[i] += v2[j] * matrix[i + j * 4];
  					}
  				}
  				gxVertex4fv(v1_t);
  				gxVertex4fv(v2_t);
  			}
  		}
  		gxEnd();
  		
		// Polygon normals.

  		gxColor3ub(0, 0, 255);
  		gxBegin(GX_LINES);
  		{
  			for (int i = 0; i < (int)base.polygon.size(); ++i)
  			{
  				float v1[4] =
  				{
  					base.polygon[i].center[0],
  					base.polygon[i].center[1],
  					base.polygon[i].center[2],
					1.f
  				};
  				float v2[4] =
  				{
  					base.polygon[i].center[0] + base.polygon[i].plane.normal[0] * 0.1f,
  					base.polygon[i].center[1] + base.polygon[i].plane.normal[1] * 0.1f,
  					base.polygon[i].center[2] + base.polygon[i].plane.normal[2] * 0.1f,
					1.f
  				};
  				float v1_t[4];
  				float v2_t[4];
  				for (int i = 0; i < 4; ++i)
  				{
  					v1_t[i] = 0.f;
  					v2_t[i] = 0.f;
  					for (int j = 0; j < 4; ++j)
  					{
  						v1_t[i] += v1[j] * matrix[i + j * 4];
  						v2_t[i] += v2[j] * matrix[i + j * 4];
  					}
  				}
  				gxVertex4fv(v1_t);
  				gxVertex4fv(v2_t);
  			}
  		}
  		gxEnd();
		
		#if 1
		
		popWireframe();
		
		#endif
		
		framework.endDraw();
		
		time += framework.timeStep;
	
	}

	return 0;

}

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
			{ 0.f, 0.f, 1.f },
			{ 0.f, 1.f, 0.f }
		},
		{
			{ 0.f, 0.f, 1.f },
			{ 1.f, 0.f, 0.f }
		},
		{
			{ 1.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f }
		}
	};

	// Find best texture plane.

	int plane;
	if (std::abs(poly.plane.normal[0]) > std::abs(poly.plane.normal[1]))
		plane = 0;
	else
		plane = 1;
	if (std::abs(poly.plane.normal[2]) > std::abs(poly.plane.normal[plane]))
		plane = 2;

	for (CEdge* edge = poly.edgeHead; edge; edge = edge->next)
	{

		// Apply texture plane to get texture coordinates.

		for (int i=0; i<2; i++)
			edge->t[0][i] = (uv[plane][i][0] * edge->p[0] + uv[plane][i][1] * edge->p[1] + uv[plane][i][2] * edge->p[2]);

	}

}
