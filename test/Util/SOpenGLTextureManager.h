#ifndef __STextureManager_h__
#define __STextureManager_h__

#if 0

#include <gl/gl.h>
#include <vector>

class COpenGLTexture
{

	public:
	
	COpenGLTexture()
	{
		glGenTextures(1, &handle);
		
		colourMap = 0;
	}
	
	~COpenGLTexture()
	{
		glDeleteTextures(1, &handle);
		
		copyColourMap(0);
	}
	
	public:
	
	GLuint handle;
	
	BITMAP* colourMap;
	
	public:
	
	void copyColourMap(BITMAP* colourMap)
	{
	
		if (this->colourMap)
			destroy_bitmap(this->colourMap);
		this->colourMap = 0;
		if (!colourMap)
			return;
		this->colourMap = create_bitmap_ex(32, colourMap->w, colourMap->h);
		blit(colourMap, this->colourMap, 0, 0, 0, 0, colourMap->w, colourMap->h);
		
	}
	
	void uploadColourMap()
	{

		if (!colourMap)
			return;
			
		if (colourMap->h > 1)
		{

			glBindTexture(GL_TEXTURE_2D, handle);
//			glTexImage2D(GL_TEXTURE_2D, 0, 4, colourMap->w, colourMap->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, colourMap->line[0]);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, colourMap->w, colourMap->h, GL_RGBA, GL_UNSIGNED_BYTE, colourMap->line[0]);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		}
		else
		{

			glBindTexture(GL_TEXTURE_1D, handle);
			glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, colourMap->w, 0, GL_RGBA, GL_UNSIGNED_BYTE, colourMap->line[0]);

			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		}
	
	}

};

class SOpenGLTextureManager
{

	private:
	
	SOpenGLTextureManager()
	{
	
	}
	
	public:
	
	~SOpenGLTextureManager()
	{
	
		while (texture.size() > 0)
		{
			delete texture[texture.size() - 1];
			texture.pop_back();
		}
	
	}
	
	public:
	
	static SOpenGLTextureManager& I()
	{
		static SOpenGLTextureManager openGLTextureManager;
		return openGLTextureManager;
	}
	
	public:
	
	std::vector<COpenGLTexture*> texture;
	
	public:

	GLuint addTextureFromFile(char* filename)
	{
	
		PALETTE palette;
		
		BITMAP* temp = load_bitmap(filename, palette);
		
		select_palette(palette);
		
		GLuint handle = addTextureFromBitmap(temp);
		
		unselect_palette();
		
		destroy_bitmap(temp);
		
		return handle;

	}
	
	GLuint addTextureFromBitmap(BITMAP* colourMap)
	{
	
		COpenGLTexture* openGLTexture = new COpenGLTexture;
		
		openGLTexture->copyColourMap(colourMap);
		
		openGLTexture->uploadColourMap();
		
		return openGLTexture->handle;
	
	}
	
};

#endif

#endif
