#pragma once
#ifndef _RenderTexture_h_
#define _RenderTexture_h_
#include <GL/glew.h>
struct RenderTexture
{
	RenderTexture(int _width, int _height);
	~RenderTexture();

	void DefaultMap();
	void bind();
	void unbind();
	GLuint getTexture();

private:
	GLuint m_fboId;
	GLuint m_texId;
	GLuint m_rboId;
	int m_width;
	int m_height;

};
#endif