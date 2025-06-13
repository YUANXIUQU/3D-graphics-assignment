#include "RenderTexture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
RenderTexture::RenderTexture(int _width, int _height) :
	m_fboId(0),
	m_texId(0),
	m_rboId(0)
{
	m_width = _width;
    m_height = _height;
}
// set up the framebuffer, texture, and renderbuffer
void RenderTexture::DefaultMap()
{
	glGenFramebuffers(1, &m_fboId);
	if (m_fboId == 0) {
		throw std::exception();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);

	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texId, 0);

	glGenRenderbuffers(1, &m_rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rboId);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


RenderTexture::~RenderTexture()
{
	glDeleteFramebuffers(1,&m_fboId);
	glDeleteTextures(1,&m_texId);
	glDeleteRenderbuffers(1,&m_rboId);
}

void RenderTexture::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
	
}

void RenderTexture::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint RenderTexture::getTexture()
{
	return m_texId;
}

