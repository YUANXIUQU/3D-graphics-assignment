#pragma once
#include <iostream>
#include "GameObject.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Skybox :public GameObject
{
private:
	VBO m_Pos;
	VAO m_Vao;
	Texture m_Tex;
public:
	Skybox(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void draw(ShaderProgram& shader, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos);
};


inline Skybox::Skybox(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
	GameObject(position, rotation, scale)
{
    // File names for the cube map faces (px = positive x, nx = negative x ..)
    std::vector<std::string> faces
    {
            "px.png",
            "nx.png",
            "py.png",
            "ny.png",
            "pz.png",
            "nz.png"
    };
    // Load the cube map texture
    m_Tex.load(faces);
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };                           
    // vertex data from website:https://learnopengl.com/Advanced-OpenGL/Cubemaps
    int size = sizeof(skyboxVertices) / sizeof(skyboxVertices[0]);
    m_Pos.add(skyboxVertices, size, 3);
    m_Vao.setBuffer(&m_Pos);
}

inline void Skybox::draw(ShaderProgram& shader, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos)
{
    shader.drawSkybox(&m_Vao, m_Tex.cubemap_id(), model, view, projection, cameraPos);
}