#include "SOpenGL.h"

class SOpenGLRender
{

	public:

	static void render(CCompiledMesh& mesh, CCompiledMesh& base);
	static void renderVertexNormals(CCompiledMesh& mesh, CCompiledMesh& base);
	static void renderPolygonNormals(CCompiledMesh& mesh, CCompiledMesh& base);

};

void SOpenGLRender::render(CCompiledMesh& mesh, CCompiledMesh& base)
{

	for (int i = 0; i < (int)mesh.polygonIndex.size(); ++i)
	{

			glColor3ub(255, 255, 255);
      		glBegin(GL_POLYGON);
      		{
      			int p = mesh.polygonIndex[i];
      			for (int j = 0; j < (int)base.polygon[p].vertex.size(); ++j)
      			{
//   					glColor3fv(n);
					glTexCoord2f(base.texcoord[base.polygon[p].texcoord[j]].t[0], base.texcoord[base.polygon[p].texcoord[j]].t[1]);
      				glNormal3fv(&base.vertex[base.polygon[p].vertex[j]].plane.normal[0]);
      				glVertex4fv(&base.vertex[base.polygon[p].vertex[j]].p[0]);
      			}
      		}
      		glEnd();
		}

}

#if 0
void SOpenGLRender::renderVertexNormals(CCompiledMesh& mesh, CCompiledMesh& base)
{

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
  		
}
#endif

void SOpenGLRender::renderPolygonNormals(CCompiledMesh& mesh, CCompiledMesh& base)
{

	glBegin(GL_LINES);
 	{

		for (int i = 0; i < (int)mesh.polygonIndex.size(); ++i)
  		{

			int j = mesh.polygonIndex[i];

			float tmp[3];
			for (int k = 0; k < 3; ++k)
    				tmp[k] = base.polygon[j].center[k] + base.polygon[j].plane.normal[k] * 0.1;

			glVertex3fv(base.polygon[j].center);
			glVertex3fv(tmp);

  		}
  	}
  	glEnd();

}
