#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "VBO.h"
struct VAO
{
	VAO();
	~VAO();
	void setBuffer(VBO* buffer);
	int getVertexCount();
	GLuint id();
private:
	GLuint m_id;
	std::vector<VBO*> buffers;
	bool m_dirty;
};