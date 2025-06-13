#include "ShaderProgram.h"
#include "Texture.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

ShaderProgram::ShaderProgram(const char* vert, const char* frag)
{
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vert, NULL);
    glCompileShader(vertexShaderId);
    GLint success = 0;
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        throw std::exception();
    }
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &frag, NULL);
    glCompileShader(fragmentShaderId);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragmentShaderId, maxLength, &maxLength, &errorLog[0]);

        std::cout << &errorLog[0] << std::endl;
        throw std::exception();
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShaderId);
    glAttachShader(m_id, fragmentShaderId);

    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);

    if (!success)
    {
        throw std::exception();
    }

    glDetachShader(m_id, vertexShaderId);
    glDeleteShader(vertexShaderId);
    glDetachShader(m_id, fragmentShaderId);
    glDeleteShader(fragmentShaderId);
}

// Draw function for rendering with VAO and Texture (used in 3D rendering)
void ShaderProgram::draw(VAO* vertexArray,Texture* tex, glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos)
{
    glUseProgram(this->getId());

    glBindVertexArray(vertexArray->id());
    glBindTexture(GL_TEXTURE_2D, tex->id());

    this->setUniform("u_View", view);
    this->setUniform("u_Model", model);
    this->setUniform("u_Projection", projection);
    this->setUniform("u_ViewPos", cameraPos);

    glDrawArrays(GL_QUADS, 0, vertexArray->getVertexCount());
    glBindVertexArray(0);
    glUseProgram(0);
}

// Draw function for rendering a Model
void ShaderProgram::draw(Model* mod, Texture* tex, glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos)
{
    glUseProgram(this->getId());

    glBindVertexArray(mod->vao_id());
    glBindTexture(GL_TEXTURE_2D, tex->id());

    this->setUniform("u_View", view);
    this->setUniform("u_Model", model);
    this->setUniform("u_Projection", projection);
    this->setUniform("u_ViewPos", cameraPos);
    glDrawArrays(GL_TRIANGLES, 0, mod->vertex_count());

    glBindVertexArray(0);
    glUseProgram(0);
}

// Draw function for rendering 2D UI elements (no view matrix)
void ShaderProgram::draw(VAO* vertexArray, GLuint tex_id, glm::mat4& model, glm::mat4& projection)
{
    glUseProgram(this->getId());
    glDepthMask(GL_FALSE);
    glBindVertexArray(vertexArray->id());
    glBindTexture(GL_TEXTURE_2D, tex_id);

    this->setUniform("u_Model", model);
    this->setUniform("u_Projection", projection);
    glDrawArrays(GL_QUADS, 0, vertexArray->getVertexCount());
    glDepthMask(GL_TRUE);
    glBindVertexArray(0);
    glUseProgram(0);
}

// Draw function specifically for skybox rendering
void ShaderProgram::drawSkybox(VAO* vertexArray, GLuint tex_id, glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos)
{
    glUseProgram(this->getId());
    glDepthMask(GL_FALSE);
    glBindVertexArray(vertexArray->id());
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);

    this->setUniform("u_View", view);
    this->setUniform("u_Model", model);
    this->setUniform("u_Projection", projection);
    this->setUniform("u_ViewPos", cameraPos);

    glDrawArrays(GL_TRIANGLES, 0, vertexArray->getVertexCount());
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glUseProgram(0);
}

GLuint ShaderProgram::getId()
{
    return m_id;
}

// set different types of uniform variables
void ShaderProgram::setUniform(std::string uniform, glm::mat4 value)
{
    GLint loc = glGetUniformLocation(m_id, uniform.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
void ShaderProgram::setUniform(std::string uniform, glm::vec2 value)
{
    GLint loc = glGetUniformLocation(m_id, uniform.c_str());
    glUniform2fv(loc, 1, glm::value_ptr(value));
}
void ShaderProgram::setUniform(std::string uniform, glm::vec3 value)
{
    GLint loc = glGetUniformLocation(m_id, uniform.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(value));
}
void ShaderProgram::setUniform(std::string uniform, glm::vec4 value)
{
    GLint loc = glGetUniformLocation(m_id, uniform.c_str());
    glUniform4f(loc, value.x,value.y,value.z,value.w);
}

void ShaderProgram::setUniform(std::string uniform, float value)
{
    GLint loc = glGetUniformLocation(m_id, uniform.c_str());
    glUniform1f(loc, value);
}

