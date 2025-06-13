#include "VAO.h"
VAO::VAO():m_dirty(false),m_id(0)
{
}

VAO::~VAO()
{
	if (m_id != 0) {
		glDeleteVertexArrays(1, &m_id);
	}
}

void VAO::setBuffer(VBO* buffer)
{
	buffers.push_back(buffer);
	m_dirty = true;
}

int VAO::getVertexCount()
{
	if (buffers.empty()) return 0;
	return buffers[0]->getDataSize() / (buffers[0]->getComponents() * sizeof(GLfloat));
	// get vertexcount from position vbo 
}

GLuint VAO::id()
{
	if (!m_id)
	{
		glGenVertexArrays(1, &m_id);
	}
	if (m_dirty)
	{
		glBindVertexArray(m_id);
		for (size_t i = 0; i < buffers.size(); ++i)
		{
			glBindBuffer(GL_ARRAY_BUFFER, buffers[i]->id());
			glVertexAttribPointer(i,buffers[i]->getComponents(), GL_FLOAT, GL_FALSE,
				buffers[i]->getComponents() * sizeof(GLfloat), (void*)0);

			glEnableVertexAttribArray(i);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		m_dirty = false;
	}
	return m_id;
}