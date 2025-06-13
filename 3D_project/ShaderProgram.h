#pragma once
#include<string>
#include "VAO.h"
#include "Model.h"
#include "Texture.h"
struct ShaderProgram
{
	ShaderProgram(const char* vert, const char* frag);
	void draw(VAO* vertexArray,Texture* tex, glm::mat4& model,glm::mat4& view,glm::mat4& projection,glm::vec3& cameraPos);
	void draw(Model* mod, Texture* tex, glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos);
	void draw(VAO* vertexArray, GLuint tex_id, glm::mat4& model, glm::mat4& projection);
	void drawSkybox(VAO* vertexArray, GLuint tex_id, glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos);

	void setUniform(std::string uniform, glm::mat4 value);
	void setUniform(std::string uniform, glm::vec2 value);
	void setUniform(std::string uniform, glm::vec3 value);
	void setUniform(std::string uniform, glm::vec4 value);
	void setUniform(std::string uniform, float value);
	GLuint getId();

private:
	GLuint m_id;
};
