#include "VBO.h"

VBO::VBO() : m_dirty(false), m_id(0) ,components(0)
{
    
}

VBO::~VBO()
{
    if(m_id != 0)
    {
        glDeleteBuffers(1, &m_id);
    }
}

// add 2d vector to the VBO data (x, y)
void VBO::add(glm::vec2 value)
{
    m_data.push_back(value.x);
    m_data.push_back(value.y);
    components = 2;
    m_dirty = true;
}

// add 3d vector to the VBO data (x, y, z)
void VBO::add(glm::vec3 value)
{
    m_data.push_back(value.x);
    m_data.push_back(value.y);
    m_data.push_back(value.z);
    components = 3;
    m_dirty = true;
}

// add 4d vector to the VBO data (x, y, z, w)
void VBO::add(glm::vec4 value)
{
    m_data.push_back(value.x);
    m_data.push_back(value.y);
    m_data.push_back(value.z);
    m_data.push_back(value.w);
    components = 4;
    m_dirty = true;
}

// add vertex data from an array of floats
void VBO::add(float* vert, int size, int v_type)
{
    for (int i = 0; i < size; ++i)
    {
        m_data.push_back(vert[i]);
    }
    components = v_type; // Set the number of components per vertex (2, 3, or 4)
    m_dirty = true;
}

int VBO::getDataSize()
{
    return m_data.size() * sizeof(GLfloat);
}

// get the number of components per vertex (2 for vec2, 3 for vec3...)
int VBO::getComponents()
{
    return components;
}

GLuint VBO::id()
{
    if (!m_id)
    {
        glGenBuffers(1, &m_id);
    }
    if (m_dirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, getDataSize(), m_data.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        m_dirty = false;
    }
    return m_id;
}