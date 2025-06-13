#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct VBO
{
	VBO();
	~VBO();
	void add(glm::vec2 value);
	void add(glm::vec3 value);
	void add(glm::vec4 value);
	void add(float* vert, int size, int v_type);
	int getDataSize();
	int getComponents();
	GLuint id();
private:
	bool m_dirty;
	std::vector<GLfloat> m_data;
	GLuint m_id;
	int components;
};